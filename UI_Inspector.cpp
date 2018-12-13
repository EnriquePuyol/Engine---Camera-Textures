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

	int numElements = 3;
	int padding = 14;
	const char * title = "Add Component...";
	static bool pressed = false;

	ImVec2 size = ImGui::GetWindowSize();
	ImGui::SetWindowSize(ImVec2(size.x, size.y + 20 * numElements));
	ImVec2 size2 = ImGui::GetWindowSize();

	if (nullptr == App->scene->selectedGO)
	{
		ImGui::Text("No gameobject selected...");
	}
	else
	{
		App->scene->selectedGO->DrawComponents();

		ImGui::Spacing();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize(title).x / 2);

		ImVec2 size = ImVec2(140, 20);

		if (pressed && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(0))
		{
			pressed = false;
		}

		if (ImGui::BeginButtonDropDown("Add Component...   ", size, numElements, &pressed))
		{
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize(title).x / 2);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
			if (ImGui::Button("Transform", size))
			{
				App->scene->selectedGO->AddComponent(Transform);
				pressed = false;
			}

			ImGui::SeparatorCustom(ImGui::GetWindowWidth() / 2 - (size.x / 2) + padding, size.x);
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize(title).x / 2);

			if (ImGui::Button("Mesh", size))
			{
				pressed = false;
			}

			ImGui::SeparatorCustom(ImGui::GetWindowWidth() / 2 - (size.x / 2) + padding, size.x);
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize(title).x / 2);

			if (ImGui::Button("Material", size))
			{
				pressed = false;
			}
			ImGui::PopStyleVar();

			ImGui::EndButtonDropDown();


		}
		else
		{
			ImGui::Spacing(20 * numElements);
		}

	}

	ImGui::End();
}
