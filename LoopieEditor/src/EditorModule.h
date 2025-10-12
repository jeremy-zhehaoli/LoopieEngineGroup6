#pragma once

#include "Loopie/Core/Module.h"

#include "Interfaces/Editor/InspectorInterface.h"
#include "Interfaces/Editor/ConsoleInterface.h"
#include "Interfaces/Editor/HierarchyInterface.h"
#include "Interfaces/Editor/SceneInterface.h"
#include "Interfaces/Editor/EditorMenuInterface.h"

/// Test

#include "Loopie/Core/Math.h"
#include "Loopie/Components/MeshRenderer.h"

///

namespace Loopie {
	class EditorModule : public Module {
	public:
		EditorModule() = default;
		~EditorModule() = default;

		void OnLoad()override;
		void OnUnload()override;

		void OnUpdate(float dt) override;

		void OnInterfaceRender()override;
	private:
		InspectorInterface m_inspector;
		ConsoleInterface m_console;
		HierarchyInterface m_hierarchy;
		SceneInterface m_scene;
		EditorMenuInterface m_mainMenu;


		/// Test
		std::vector<MeshRenderer*> meshRenderers;

		const float FOV = 45.0f;
		const float NEAR_PLANE = 0.1f;
		const float FAR_PLANE = 200.0f;

		float rotation = 0;
		vec3 position = vec3(0.0f, 0.0f, -50.0f);
		///
	};
}