#pragma once

#include <string>
#include <map>
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include <glm/fwd.hpp>

namespace Loopie {

    class AudioManager {
    public:
        // Ciclo de vida del sistema
        static void Init();
        static void Update();
        static void Shutdown();

        static FMOD::Studio::EventInstance* CreateEventInstance(const std::string& eventPath);

        // Actualiza la posición de los "oídos" (Cámara)
        static void SetListenerAttributes(const glm::vec3& pos, const glm::vec3& forward, const glm::vec3& up);

        // Helper para convertir vectores de GLM a FMOD (puedes ponerlo privado si quieres)
        static FMOD_VECTOR VectorToFmod(const glm::vec3& v);

        static FMOD::Sound* CreateSound(const std::string& path, bool loop = false);
        static void PlaySound(FMOD::Sound* sound, FMOD::Channel** channel);

        // Carga de bancos (Resources)
        // flags por defecto carga los samples en memoria
        static void LoadBank(const std::string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags = FMOD_STUDIO_LOAD_BANK_NORMAL);

        // API Pública para Componentes (la expandiremos luego)
        static FMOD::Studio::System* GetSystem() { return s_studioSystem; }

        // Helper para chequear errores de FMOD internamente
        static bool CheckError(FMOD_RESULT result, const char* message);

    private:
        static FMOD::Studio::System* s_studioSystem;
        static FMOD::System* s_coreSystem; // Sistema de bajo nivel (necesario a veces)

        // Guardamos los bancos cargados para no cargarlos dos veces
        static std::map<std::string, FMOD::Studio::Bank*> s_loadedBanks;
    };

}