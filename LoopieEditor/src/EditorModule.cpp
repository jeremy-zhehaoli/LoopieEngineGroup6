#include "EditorModule.h"

#include "Loopie/Core/Application.h"

//// Test
#include "Loopie/Core/Log.h"
#include "Loopie/Render/Renderer.h"
#include "Loopie/Files/MeshImporter.h"

#include "Loopie/Core/Math.h"
#include "Loopie/Resources/ResourceDatabase.h"
#include "Loopie/Resources/AssetRegistry.h"
///


#include <imgui.h>

namespace Loopie
{
	void EditorModule::OnLoad()
	{
		AssetRegistry::Initialize();
		Application::GetInstance().GetWindow().SetResizable(true);

	}

	void EditorModule::OnUnload()
	{
		AssetRegistry::Shutdown();
		for (size_t i = 0; i < meshRenderers.size(); i++)
		{
			delete meshRenderers[i];
		}
		meshRenderers.clear();
	}

	void EditorModule::OnUpdate(float dt)
	{
		InputEventManager& inputEvent = Application::GetInstance().GetInputEvent();

		const float SPEED = 100.0f;
		glm::vec3 forward(0.0f, 0.0f, 1.0f);
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		glm::mat4 viewMatrix = glm::lookAt(position, position + forward, up);

		ivec2 windowSize = Application::GetInstance().GetWindow().GetSize();

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(FOV), static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y), NEAR_PLANE, FAR_PLANE);


		if (inputEvent.HasFileBeenDropped()) {
			const char* fileName = inputEvent.GetDroppedFile(0);
			if (MeshImporter::CheckIfIsModel(fileName)) {
				//for (size_t i = 0; i < meshRenderers.size(); i++)
				//{
				//	delete meshRenderers[i];
				//}
				//meshRenderers.clear();

				//std::vector<std::shared_ptr<Mesh>> meshes = MeshImporter::LoadModel(fileName);

				//for (size_t i = 0; i < meshes.size(); i++)
				//{
				//	meshRenderers.push_back(new MeshRenderer(meshes[i]));
				//}


				///// Generate MetaData
				///// Generate MeshFromMetaData
				///// Generate MeshRenderer

				if (!AssetRegistry::AssetExists(fileName)) {
					std::vector<std::string> cacheFiles = MeshImporter::LoadModel(fileName);

					for (size_t i = 0; i < cacheFiles.size(); i++)
					{
						std::filesystem::path path = cacheFiles[i];

						AssetMetadata metadata;
						metadata.uuid = UUID(path.stem().string());
						metadata.cachePath = cacheFiles[i];
						metadata.sourcePath = fileName;
						AssetRegistry::RegisterAsset(metadata);

						std::shared_ptr<Mesh> mesh = ResourceDatabase::LoadResource<Mesh>(metadata.uuid);
						if (mesh)
							meshRenderers.push_back(new MeshRenderer(mesh));
					}
				}
				else {
					std::vector<UUID> uuids = AssetRegistry::GetUUIDFromSourcePath(fileName);
					for (size_t i = 0; i < uuids.size(); i++)
					{
						AssetMetadata* metadata = AssetRegistry::GetMetadata(uuids[i]);
						std::shared_ptr<Mesh> mesh = ResourceDatabase::LoadResource<Mesh>(metadata->uuid);
						if (mesh)
							meshRenderers.push_back(new MeshRenderer(mesh));

					}
				}

				

				
			}
		}

		if (inputEvent.GetKeyStatus(SDL_SCANCODE_W) == KeyState::REPEAT)
			position.z += 10 * dt;

		if (inputEvent.GetKeyStatus(SDL_SCANCODE_S) == KeyState::REPEAT)
			position.z -= 10 * dt;

		viewMatrix = glm::lookAt(position, position + forward, up);
		projectionMatrix = glm::perspective(glm::radians(FOV), static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y), NEAR_PLANE, FAR_PLANE);
		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 modelViewProj = projectionMatrix * viewMatrix * modelMatrix;

		rotation += SPEED * dt;

		for (size_t i = 0; i < meshRenderers.size(); i++)
		{
			meshRenderers[i]->GetShader().Bind();
			meshRenderers[i]->GetShader().SetUniformMat4("modelViewProj", modelViewProj);
			meshRenderers[i]->Render();
		}

	}

	void EditorModule::OnInterfaceRender()
	{
		ImGui::DockSpaceOverViewport();

		m_mainMenu.Render();
		m_inspector.Render();
		m_console.Render();
		m_hierarchy.Render();
		m_scene.Render();
	}
}