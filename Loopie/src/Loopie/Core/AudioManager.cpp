#include "AudioManager.h"
#include "Loopie/Core/Log.h"
#include <fmod_errors.h>
#include <filesystem>

namespace Loopie {

    FMOD::Studio::System* AudioManager::s_studioSystem = nullptr;
    FMOD::System* AudioManager::s_coreSystem = nullptr;

    void AudioManager::Init() {
        // Inicializar FMOD Studio y Core
        FMOD_RESULT result = FMOD::Studio::System::create(&s_studioSystem);
        if (result != FMOD_OK) {
            Log::Critical("FMOD Create Error: {0}", FMOD_ErrorString(result));
            return;
        }

        s_studioSystem->getCoreSystem(&s_coreSystem);

        result = s_studioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
        if (result != FMOD_OK) {
            Log::Critical("FMOD Init Error: {0}", FMOD_ErrorString(result));
            return;
        }

        Log::Info("Audio Manager (FMOD) Initialized.");
    }

    void AudioManager::Update() {
        if (s_studioSystem) {
            s_studioSystem->update();
        }
    }

    void AudioManager::Shutdown() {
        if (s_studioSystem) {
            s_studioSystem->unloadAll();
            s_studioSystem->release();
        }
    }

    FMOD::Studio::EventInstance* AudioManager::CreateEventInstance(const std::string& eventPath) {
        FMOD::Studio::EventDescription* eventDesc = nullptr;
        FMOD_RESULT result = s_studioSystem->getEvent(eventPath.c_str(), &eventDesc);

        if (result != FMOD_OK) {
            Log::Error("FMOD Event not found: {0}", eventPath);
            return nullptr;
        }

        FMOD::Studio::EventInstance* instance = nullptr;
        eventDesc->createInstance(&instance);
        return instance;
    }

    FMOD::Sound* AudioManager::CreateSound(const std::string& path, bool loop) {
        FMOD::Sound* sound = nullptr;

        // Configuración 3D por defecto
        FMOD_MODE mode = FMOD_DEFAULT | FMOD_3D;

        // Aplicar flag de Loop
        if (loop) mode |= FMOD_LOOP_NORMAL;
        else mode |= FMOD_LOOP_OFF;

        // INTENTO 1: Ruta tal cual (relativa al .exe)
        FMOD_RESULT result = s_coreSystem->createSound(path.c_str(), mode, nullptr, &sound);

        // INTENTO 2: Corrección de ruta para Debug (subir 2 niveles a 'assets')
        if (result != FMOD_OK) {
            std::string fixPath = "../../" + path;
            result = s_coreSystem->createSound(fixPath.c_str(), mode, nullptr, &sound);

            if (result == FMOD_OK) {
                // Log::Info("Audio loaded via fixPath: {0}", fixPath); // Descomenta para debug
            }
        }

        if (result != FMOD_OK) {
            Log::Error("Failed to load sound: {0}. FMOD Error: {1}", path, FMOD_ErrorString(result));
            return nullptr;
        }

        return sound;
    }

    void AudioManager::PlaySound(FMOD::Sound* sound, FMOD::Channel** channel) {
        if (s_coreSystem && sound) {
            s_coreSystem->playSound(sound, nullptr, false, channel);
        }
    }

    void AudioManager::SetListenerAttributes(const glm::vec3& pos, const glm::vec3& forward, const glm::vec3& up) {
        FMOD_3D_ATTRIBUTES attributes = { {0} };
        attributes.position = VectorToFmod(pos);
        attributes.forward = VectorToFmod(forward);
        attributes.up = VectorToFmod(up);

        s_studioSystem->setListenerAttributes(0, &attributes);
    }

    void AudioManager::SetGlobalParameter(const std::string& name, float value) {
        if (s_studioSystem) {
            s_studioSystem->setParameterByName(name.c_str(), value);
        }
    }

    FMOD_VECTOR AudioManager::VectorToFmod(const glm::vec3& v) {
        return { v.x, v.y, v.z };
    }
}