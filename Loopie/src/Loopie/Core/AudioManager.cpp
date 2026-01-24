#include "AudioManager.h"
#include "Loopie/Core/Log.h" // Usamos tu sistema de Log
#include <glm/glm.hpp> // Asegúrate de incluir GLM
#include <filesystem>


namespace Loopie {

    FMOD::Studio::System* AudioManager::s_studioSystem = nullptr;
    FMOD::System* AudioManager::s_coreSystem = nullptr;
    std::map<std::string, FMOD::Studio::Bank*> AudioManager::s_loadedBanks;

    void AudioManager::Init() {
        Log::Info("Initializing Audio Manager (FMOD)...");

        // 1. Crear el objeto del sistema Studio
        FMOD_RESULT result = FMOD::Studio::System::create(&s_studioSystem);
        if (!CheckError(result, "FMOD Studio System creation failed")) return;

        // 2. Obtener el sistema Core (low-level)
        s_studioSystem->getCoreSystem(&s_coreSystem);

        // 3. Inicializar
        // 512 canales virtuales, flags normales, sin datos extra de driver
        result = s_studioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
        if (!CheckError(result, "FMOD initialization failed")) return;

        // 4. Cargar Bancos Maestros (Opcional: hacerlo aquí o manual en la App)
        // Se asume que los .bank están junto al .exe o en assets/audio/
        // Primero SIEMPRE cargar el Strings para poder usar nombres
        LoadBank("assets/audio/Master.strings.bank");
        LoadBank("assets/audio/Master.bank");

        Log::Info("Audio Manager Initialized.");
    }

    void AudioManager::Update() {
        if (s_studioSystem) {
            // FMOD necesita actualizarse cada frame para procesar eventos 3D, fades, etc.
            s_studioSystem->update();
        }
    }

    void AudioManager::Shutdown() {
        Log::Info("Shutting down Audio Manager...");

        // Descargar todos los bancos
        for (auto& pair : s_loadedBanks) {
            pair.second->unload();
        }
        s_loadedBanks.clear();

        // Liberar sistema
        if (s_studioSystem) {
            s_studioSystem->release();
            s_studioSystem = nullptr;
        }
    }

    FMOD_VECTOR AudioManager::VectorToFmod(const glm::vec3& v) {
        return { v.x, v.y, v.z };
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

    void AudioManager::SetListenerAttributes(const glm::vec3& pos, const glm::vec3& forward, const glm::vec3& up) {
        FMOD_3D_ATTRIBUTES attributes = { {0} };
        attributes.position = VectorToFmod(pos);
        attributes.forward = VectorToFmod(forward);
        attributes.up = VectorToFmod(up);

        // Listener 0 es el principal
        s_studioSystem->setListenerAttributes(0, &attributes);
    }

    FMOD::Sound* AudioManager::CreateSound(const std::string& path, bool loop) {
        FMOD::Sound* sound = nullptr;
        FMOD_MODE mode = FMOD_DEFAULT | FMOD_3D; // 3D enabled
        if (loop) mode |= FMOD_LOOP_NORMAL;
        else mode |= FMOD_LOOP_OFF;

        // INTENTO 1: Ruta directa (funciona si assets está junto al exe)
        FMOD_RESULT result = s_coreSystem->createSound(path.c_str(), mode, nullptr, &sound);

        // INTENTO 2: Ruta corregida (para Visual Studio / CMake)
        if (result != FMOD_OK) {
            // Probamos subiendo 2 niveles: build/Debug/ -> ../../assets/...
            std::string fixPath = "../../" + path;
            result = s_coreSystem->createSound(fixPath.c_str(), mode, nullptr, &sound);

            if (result == FMOD_OK) {
                Log::Info("Audio loaded with fix: {0}", fixPath);
            }
            else {
                // INTENTO 3: Ruta absoluta (para casos desesperados)
                std::filesystem::path absPath = std::filesystem::absolute(path);
                result = s_coreSystem->createSound(absPath.string().c_str(), mode, nullptr, &sound);
            }
        }

        if (result != FMOD_OK) {
            // ESTE ERROR ES VITAL: Míralo en la consola si no suena
            Log::Error("FMOD Failed to load: {0} (Error: {1})", path, (int)result);
            return nullptr;
        }

        return sound;
    }

    void AudioManager::PlaySound(FMOD::Sound* sound, FMOD::Channel** channel) {
        // Reproducimos en el canal master group (nullptr)
        s_coreSystem->playSound(sound, nullptr, false, channel);
    }

    void AudioManager::LoadBank(const std::string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags) {
        if (s_loadedBanks.find(bankName) != s_loadedBanks.end()) {
            return; // Ya está cargado
        }

        FMOD::Studio::Bank* bank = nullptr;
        FMOD_RESULT result = s_studioSystem->loadBankFile(bankName.c_str(), flags, &bank);

        if (CheckError(result, ("Failed to load bank: " + bankName).c_str())) {
            s_loadedBanks[bankName] = bank;
            Log::Info("Loaded Bank: {0}", bankName);
        }
    }

    bool AudioManager::CheckError(FMOD_RESULT result, const char* message) {
        if (result != FMOD_OK) {
            Log::Error("{0} - Error: {1}", message, (int)result);
            // Podrías usar FMOD_ErrorString(result) si incluyes fmod_errors.h
            return false;
        }
        return true;
    }
}