#pragma once
#include "Loopie/Components/Component.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Core/AudioManager.h"
#include "Loopie/Core/Log.h"

namespace Loopie {

    class AudioListener : public Component {
    public:
        DEFINE_TYPE(AudioListener)

        void Init() override {
            Log::Info("AudioListener INICIALIZADO. ¡Estoy vivo!");
        }

        void OnStart() override {}

        void OnUpdate() override {
            Transform* t = GetOwner()->GetTransform();
            if (t) {
                // 1. Enviar posición, vector "hacia delante" y "hacia arriba" al Audio Manager
                AudioManager::SetListenerAttributes(
                    t->GetPosition(),
                    t->Forward(),
                    t->Up()
                );

                // 2. DEBUG: Imprimir posición en consola para ver si cambia
                // Solo imprimimos si la posición NO es (0,0,0) para no saturar si está quieto
                glm::vec3 p = t->GetPosition();
                if (p.x >= 0 || p.z >= 0) {
                    // Imprimimos cada 60 frames aprox para no colgar la consola
                    static int frameCount = 0;
                    if (frameCount++ % 60 == 0) {
                        Log::Info("OIDOS EN: X={0}, Z={1}", p.x, p.z);
                    }
                }
            }
        }

        // Serialización vacía
        JsonNode Serialize(JsonNode& parent) const override { return parent; }
        void Deserialize(const JsonNode& data) override {}
    };
}