#include "UI_Inspector.h"
#include "Application.h"
#include "ModuleScene.h"

UI_Inspector::UI_Inspector(char* name) : UI(name)
{
	active = true;
}


UI_Inspector::~UI_Inspector()
{
}

void UI_Inspector::Draw()
{
	ImGui::Begin(name, &active, ImGuiWindowFlags_HorizontalScrollbar);

	if (nullptr == App->scene->selectedGO)
	{
		ImGui::Text("No gameobject selected...");
	}
	else
	{
		App->scene->selectedGO->DrawComponents();

		ImGui::Spacing();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("Add Component...").x / 2);

		ImVec2 totalSize = ImVec2(140, 20);
		ImVec2 elementSize = ImVec2(140, 20);

		if (ImGui::BeginButtonDropDown("Add Component...   ", totalSize, 3))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
			if (ImGui::Button("Transform", elementSize))
			{
				App->scene->selectedGO->AddComponent(Transform);
				ImGui::CloseCurrentPopup();
			}
			ImGui::Separator();
			if (ImGui::Button("Mesh", elementSize))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::Separator();
			if (ImGui::Button("Material", elementSize))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopStyleVar(2);

			ImGui::EndButtonDropDown();
		}
	}

	ImGui::End();
}
