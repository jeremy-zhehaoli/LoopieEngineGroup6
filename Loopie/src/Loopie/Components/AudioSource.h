#pragma once
#include "Loopie/Components/Component.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Core/AudioManager.h"
#include "Loopie/Core/Log.h"
#include <string>

namespace Loopie {

    class AudioSource : public Component {
    public:

        DEFINE_TYPE(AudioSource)

            std::string eventPath;
        bool playOnAwake = true;

    private:
        // Punteros para los dos sistemas (Usamos uno u otro)
        FMOD::Studio::EventInstance* m_eventInstance = nullptr; // Para Eventos

        FMOD::Sound* m_sound = nullptr;       // Para archivos raw
        FMOD::Channel* m_channel = nullptr;   // Canal para controlar el archivo raw

        bool m_isEvent = false; // Flag para saber qué modo usar

        void LoadResource() {
            // 1. Limpiar lo anterior si existía
            if (m_eventInstance) {
                m_eventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
                m_eventInstance->release();
                m_eventInstance = nullptr;
            }
            if (m_sound) {
                m_sound->release();
                m_sound = nullptr;
            }

            if (eventPath.empty()) return;

            // 2. Detectar tipo y Cargar
            if (eventPath.find("event:/") == 0 || eventPath.find("snapshot:/") == 0) {
                m_isEvent = true;
                m_eventInstance = AudioManager::CreateEventInstance(eventPath);
            }
            else {
                m_isEvent = false;
                // IMPORTANTE: Asegúrate de que CreateSound maneje rutas relativas (lo vemos en el Paso 2)
                m_sound = AudioManager::CreateSound(eventPath, true);
            }
        }

    public:
        // MODIFICADO: Ahora recarga el sonido al cambiar el path
        void SetEventPath(const std::string& path) {
            if (eventPath != path) { // Solo si cambia
                eventPath = path;
                LoadResource(); // <--- ¡AQUÍ ESTÁ LA CLAVE!

                // Opcional: Si quieres que suene nada más soltarlo
                // if (playOnAwake) Play(); 
            }
        }

        void Init() override {}

        void OnStart() override {
            LoadResource(); // Usamos el mismo método
            if (playOnAwake) Play();
        }

        void OnUpdate() override {
            Transform* t = GetOwner()->GetTransform();
            if (!t) return;

            // Calcular posición para FMOD
            FMOD_VECTOR pos = AudioManager::VectorToFmod(t->GetPosition());
            FMOD_VECTOR vel = { 0, 0, 0 };

            if (m_isEvent && m_eventInstance) {
                // Lógica de Eventos
                FMOD_3D_ATTRIBUTES attr = { {0} };
                attr.position = pos;
                attr.forward = AudioManager::VectorToFmod(t->Forward());
                attr.up = AudioManager::VectorToFmod(t->Up());
                m_eventInstance->set3DAttributes(&attr);
            }
            else if (!m_isEvent && m_channel) {
                // Lógica de Archivos Raw (Core System)
                // Es importante comprobar si el canal sigue activo
                bool isPlaying = false;
                m_channel->isPlaying(&isPlaying);
                if (isPlaying) {
                    m_channel->set3DAttributes(&pos, &vel);
                }
            }
        }

        void Play() {
            if (m_isEvent && m_eventInstance) {
                m_eventInstance->start();
            }
            else if (!m_isEvent && m_sound) {
                AudioManager::PlaySound(m_sound, &m_channel);
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

        // Serialización básica
        JsonNode Serialize(JsonNode& parent) const override { return parent; }
        void Deserialize(const JsonNode& data) override {}

        ~AudioSource() {
            if (m_eventInstance) {
                m_eventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
                m_eventInstance->release();
            }
            if (m_sound) {
                m_sound->release(); // Liberar el sonido raw
            }
        }
    };
}