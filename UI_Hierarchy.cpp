#include "UI_Hierarchy.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL.h"
#include "GameObject.h"
#include "ModuleScene.h"


UI_Hierarchy::UI_Hierarchy(char* name) : UI(name)
{
	active = true;
}


UI_Hierarchy::~UI_Hierarchy()
{
}

void UI_Hierarchy::Draw()
{
	isItemClicked = false;

	ImGui::Begin(name, &active, ImGuiWindowFlags_HorizontalScrollbar);

	for (list<GameObject*>::iterator it = App->scene->gameobjects.begin(); it != App->scene->gameobjects.end(); ++it)
	{
		(*it)->Draw();
	}

	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::MenuItem("Create Empty")) {}

		if (nullptr != App->scene->selectedGO)
		{
			ImGui::Separator();
			if (ImGui::MenuItem("Copy")) {}
			if (ImGui::MenuItem("Cut")) {}
			if (ImGui::MenuItem("Paste")) {}
		}
		else
		{
			ImGui::Separator();
			if (ImGui::MenuItem("Paste")) {}
		}
		if (nullptr != App->scene->selectedGO)
		{
			ImGui::Separator();
			if (ImGui::MenuItem("Delete")) {}
		}

		ImGui::EndPopup();
	}

	if (ImGui::IsMouseClicked(0) && nullptr != App->scene->selectedGO && !isItemClicked && ImGui::IsMouseHoveringWindow())
	{
		App->scene->selectedGO->selected = !App->scene->selectedGO->selected;
		App->scene->selectedGO = nullptr;
	}

	ImGui::End();
}