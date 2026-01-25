#pragma once
#include "Loopie/Components/Component.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Core/Application.h" // Para pillar el tiempo (dt)

namespace Loopie {

    class AutoMovement : public Component {
    public:
        DEFINE_TYPE(AutoMovement)

            // Velocidad de movimiento (unidades por segundo)
            float speed = 2.0f;

        // Límite para teletransportarlo al inicio (Loop de movimiento)
        float limitX = 20.0f;
        float startX = 0.0f;

        void Init() override {}
        void OnStart() override {}

        void Update() override {
            Transform* t = GetOwner()->GetTransform();
            if (!t) return;

            // 1. Obtenemos el Delta Time (tiempo entre frames) para que vaya suave
            // Si tu Application no tiene GetDeltaTime(), usa un valor fijo como 0.016f
            float dt = Application::GetInstance().GetDeltaTime();

            // 2. Mover en el eje X
            glm::vec3 pos = t->GetPosition();
            pos.x += speed * dt;

            // 3. (Opcional) Si se pasa de lejos, que vuelva al principio
            // Así puedes probar el efecto túnel una y otra vez sin reiniciar
            if (pos.x > limitX) {
                pos.x = startX;
            }

            // 4. Aplicar
            t->SetPosition(pos);
        }

        // Serialización vacía (obligatoria por la herencia)
        JsonNode Serialize(JsonNode& parent) const override { return parent; }
        void Deserialize(const JsonNode& data) override {}
    };
}