#include "EditorMenuInterface.h"

#include "Loopie/Core/Application.h"

#include <imgui.h>

namespace Loopie {
	EditorMenuInterface::EditorMenuInterface() {

	}

	void EditorMenuInterface::Render() {

		ImGuiID openProjectPopUpId = ImGui::GetID("OpenProjectPopUp");
		ImGuiID createProjectPopUpId = ImGui::GetID("CreateProjectPopUp");

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
				{
					ImGui::OpenPopup(openProjectPopUpId);
				}

				if (ImGui::MenuItem("New"))
				{
					ImGui::OpenPopup(createProjectPopUpId);
				}

				if (ImGui::MenuItem("Exit"))
					Application::GetInstance().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (ImGui::BeginPopupContextItem("OpenProjectPopUp")) {
			///Render interface
			ImGui::EndPopup();

		}

		if (ImGui::BeginPopupContextItem("CreateProjectPopUp")) {
			///Render interface
			ImGui::EndPopup();
		}
		
	}
}