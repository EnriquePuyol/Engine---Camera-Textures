#include "UI_Hierarchy.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL.h"


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

	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::MenuItem("Create Empty")) {}
		if (ImGui::MenuItem("Delete")) {}

		ImGui::EndPopup();
	}

	ImGui::End();
}