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
	ImGui::Begin(name, &active);

	for (list<GameObject*>::iterator it = App->scene->gameobjects.begin(); it != App->scene->gameobjects.end(); ++it)
	{
		(*it)->Draw();
	}

	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::MenuItem("Create Empty")) {}
		if (ImGui::MenuItem("Delete")) {}

		ImGui::EndPopup();
	}

	ImGui::End();
}