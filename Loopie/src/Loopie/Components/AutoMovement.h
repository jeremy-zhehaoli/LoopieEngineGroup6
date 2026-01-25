#pragma once
#include "Loopie/Components/Component.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Core/Application.h" 

namespace Loopie {

    class AutoMovement : public Component {
    public:
        DEFINE_TYPE(AutoMovement)

            float speed = 2.0f;

        float limitX = 20.0f;
        float startX = 0.0f;

        void Init() override {}
        void OnStart() override {}

        void Update() override {
            Transform* t = GetOwner()->GetTransform();
            if (!t) return;

            float dt = Application::GetInstance().GetDeltaTime();

            glm::vec3 pos = t->GetPosition();
            pos.x += speed * dt;

            if (pos.x > limitX) {
                pos.x = startX;
            }

            t->SetPosition(pos);
        }

        JsonNode Serialize(JsonNode& parent) const override {
        
            JsonNode transformObj = parent.CreateObjectField("AutoMovement");
			return transformObj;
        
        }
        void Deserialize(const JsonNode& data) override {}
    };
}