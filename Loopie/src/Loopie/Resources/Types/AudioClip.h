#pragma once
#include "Loopie/Resources/Resource.h"

namespace Loopie {
    class AudioClip : public Resource {
    public:
        std::string eventPath; // Ej: "event:/Explosions/Grenade"

        AudioClip(const std::string& path) : Resource(ResourceType::Audio, path) {
            // Aquí podríamos procesar el path o simplemente guardarlo.
            // Lo ideal: Que el .meta file asocie este recurso a una cadena de texto de FMOD.
            eventPath = path;
        }
    };
}