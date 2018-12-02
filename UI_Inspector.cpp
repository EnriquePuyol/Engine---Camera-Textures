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
	ImGui::Begin(name, &active);

	if (nullptr == App->scene->selectedGO)
	{
		ImGui::Text("No gameobject selected...");
	}
	else
	{
		App->scene->selectedGO->DrawComponents();
	}

	ImGui::End();
}
