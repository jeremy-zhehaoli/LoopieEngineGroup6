#pragma once
#include "Loopie/Components/Component.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Core/AudioManager.h"
#include "Loopie/Core/Log.h"
#include <string>
#include <vector>

namespace Loopie {

    class AudioSource : public Component {
    public:
        DEFINE_TYPE(AudioSource)

            // Playlist
            std::vector<std::string> audioClips;
        int currentClipIndex = 0;

        // Propiedades
        bool loop = false;
        bool playOnAwake = true;

    private:
        FMOD::Studio::EventInstance* m_eventInstance = nullptr;
        FMOD::Sound* m_sound = nullptr;
        FMOD::Channel* m_channel = nullptr;
        bool m_isEvent = false;

        // Método interno para cargar/recargar recursos
        void LoadResource() {
            // 1. Limpiar recursos anteriores
            if (m_eventInstance) {
                m_eventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
                m_eventInstance->release();
                m_eventInstance = nullptr;
            }
            if (m_sound) {
                m_sound->release();
                m_sound = nullptr;
            }

            // 2. Comprobaciones de seguridad
            if (audioClips.empty()) return;
            if (currentClipIndex < 0 || currentClipIndex >= audioClips.size()) return;

            std::string path = audioClips[currentClipIndex];
            if (path.empty()) return;

            // 3. Crear recurso según tipo
            if (path.find("event:/") == 0 || path.find("snapshot:/") == 0) {
                m_isEvent = true;
                m_eventInstance = AudioManager::CreateEventInstance(path);
            }
            else {
                m_isEvent = false;
                // Pasamos el estado del loop al creador
                m_sound = AudioManager::CreateSound(path, loop);
            }
        }

    public:
        // --- Init vacío para evitar el error de UUID ---
        void Init() override {}

        void OnStart() override {
            LoadResource();
            if (playOnAwake) {
                Play();
            }
        }

        void OnUpdate() override {
            // Lógica de posicionamiento 3D
            Transform* t = GetOwner()->GetTransform();
            if (!t) return;

            FMOD_VECTOR pos = AudioManager::VectorToFmod(t->GetPosition());
            FMOD_VECTOR vel = { 0, 0, 0 };

            if (m_isEvent && m_eventInstance) {
                FMOD_3D_ATTRIBUTES attr = { {0} };
                attr.position = pos;
                attr.forward = AudioManager::VectorToFmod(t->Forward());
                attr.up = AudioManager::VectorToFmod(t->Up());
                m_eventInstance->set3DAttributes(&attr);
            }
            else if (!m_isEvent && m_channel) {
                bool isPlaying = false;
                m_channel->isPlaying(&isPlaying);
                if (isPlaying) {
                    m_channel->set3DAttributes(&pos, &vel);
                }
            }
        }

        // --- Gestión de Playlist ---

        void AddClip(const std::string& path) {
            audioClips.push_back(path);
        }

        void SetCurrentClip(int index) {
            // Solo recargamos si el índice es válido y diferente (o para forzar reload)
            if (index >= 0 && index < audioClips.size()) {
                currentClipIndex = index;

                // Si estaba sonando, que siga sonando con la nueva pista
                bool wasPlaying = false;
                if (!m_isEvent && m_channel) m_channel->isPlaying(&wasPlaying);

                LoadResource();

                if (wasPlaying) Play();
            }
        }

        // --- Controles de Reproducción ---

        void Play() {
            if (m_isEvent && m_eventInstance) {
                m_eventInstance->start();
            }
            else if (!m_isEvent && m_sound) {
                AudioManager::PlaySound(m_sound, &m_channel);
                // Aseguramos el volumen al reproducir
                // SetVolume(1.0f); 
            }
        }

        void Stop() {
            if (m_isEvent && m_eventInstance) {
                m_eventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
            }
            else if (!m_isEvent && m_channel) {
                m_channel->stop();
            }
        }

        void SetLoop(bool active) {
            loop = active;
            // Si ya está sonando un archivo raw, actualizamos el modo en caliente
            if (!m_isEvent && m_channel) {
                m_channel->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
            }
        }

        void SetVolume(float volume) {
            if (m_isEvent && m_eventInstance) {
                m_eventInstance->setVolume(volume);
            }
            else if (!m_isEvent && m_channel) {
                m_channel->setVolume(volume);
            }
        }

        void SetParameter(const std::string& name, float value) {
            if (m_isEvent && m_eventInstance) {
                m_eventInstance->setParameterByName(name.c_str(), value);
            }
        }

        // --- Serialización (Stubs) ---
        JsonNode Serialize(JsonNode& parent) const override { return parent; }
        void Deserialize(const JsonNode& data) override {}

        ~AudioSource() {
            Stop();
            if (m_eventInstance) {
                m_eventInstance->release();
                m_eventInstance = nullptr;
            }
            if (m_sound) {
                m_sound->release();
                m_sound = nullptr;
            }
        }
    };
}