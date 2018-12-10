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


		/*if (ImGui::Button("Add Component..."))
		{
		}*/
		//ImVec2 size = ImGui::GetItemRectSize();
		if (ImGui::BeginButtonDropDown("Add Component...   ", ImVec2(140, 20)))
		{
			if (ImGui::Button("Transform")) 
			{
				App->scene->selectedGO->AddComponent(Transform);
			}

			ImGui::EndButtonDropDown();
		}
	}

	ImGui::End();
}
