#include "AssetsExplorerInterface.h"

#include "Loopie/Core/Application.h"

#include <imgui.h>

namespace Loopie {
	AssetsExplorerInterface::AssetsExplorerInterface() {

	}

	void AssetsExplorerInterface::Render() {
		if (ImGui::Begin("Assets Explorer")) {
			const Project& project = Application::GetInstance().m_activeProject;
			if (project.IsEmpty()) {
				ImGui::End();
				return;
			}
			const std::filesystem::path assetPath = project.GetAssetsPath();

			if (m_currentDirectory.empty())
				m_currentDirectory = assetPath;
			


		}
		ImGui::End();
	}
}