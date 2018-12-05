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
	// Color de relleno de la ventana
	// ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.24f, 0.24f, 0.24f, 1));

	ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(0.24f, 0.24f, 0.24f, 1));

	ImGui::Begin(name, &active, ImGuiWindowFlags_HorizontalScrollbar);

	for (list<GameObject*>::iterator it = App->scene->root->childs.begin(); it != App->scene->root->childs.end(); ++it)
	{
		(*it)->Draw();
	}

	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::MenuItem("Create Empty")) { App->scene->CreateGameObject(); }

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
			if (ImGui::MenuItem("Delete")) { App->scene->DeleteGameObject(); }
		}

		ImGui::EndPopup();
	}

	if (ImGui::IsMouseClicked(0) && nullptr != App->scene->selectedGO && !isItemClicked && ImGui::IsMouseHoveringWindow())
	{
		App->scene->selectedGO->selected = !App->scene->selectedGO->selected;
		App->scene->selectedGO = nullptr;
	}

	ImGui::End();

	ImGui::PopStyleColor();
}