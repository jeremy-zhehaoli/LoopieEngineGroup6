#pragma once
#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <string>
#include <glm/glm.hpp>

namespace Loopie {

    class AudioManager {
    public:
        static void Init();
        static void Update();
        static void Shutdown();

        // Cargar/Crear recursos
        static FMOD::Studio::EventInstance* CreateEventInstance(const std::string& eventPath);
        static FMOD::Sound* CreateSound(const std::string& path, bool loop = false);

        // Control
        static void PlaySound(FMOD::Sound* sound, FMOD::Channel** channel, bool paused = false);

        // 3D Audio
        static void SetListenerAttributes(const glm::vec3& pos, const glm::vec3& forward, const glm::vec3& up);
        static FMOD_VECTOR VectorToFmod(const glm::vec3& v);

        // Parámetros globales (para el Túnel, por ejemplo)
        static void SetGlobalParameter(const std::string& name, float value);

        static FMOD::Studio::System* s_studioSystem;
        static FMOD::System* s_coreSystem;
    };
}