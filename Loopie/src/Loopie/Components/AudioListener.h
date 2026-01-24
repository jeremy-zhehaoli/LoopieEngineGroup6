#pragma once
#include "Loopie/Components/Component.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Core/AudioManager.h"

namespace Loopie {

    class AudioListener : public Component {
    public:
        // 1. MACRO
        DEFINE_TYPE(AudioListener)

            // 2. INIT
            void Init() override {}

        // 3. UPDATE
        void OnUpdate() override {
            Transform* t = GetOwner()->GetTransform();
            if (t) {
                AudioManager::SetListenerAttributes(t->GetPosition(), t->Forward(), t->Up());
            }
        }

        // 4. SERIALIZACIÓN (Stubs vacíos)
        JsonNode Serialize(JsonNode& parent) const override { return parent; }
        void Deserialize(const JsonNode& data) override {}
    };
}