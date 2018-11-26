#include "UI_Hierarchy.h"



UI_Hierarchy::UI_Hierarchy(char* name) : UI(name)
{
	//active = true;
}


UI_Hierarchy::~UI_Hierarchy()
{
}

void UI_Hierarchy::Draw()
{
	ImGui::Begin(name, &active);

	ImGui::End();
}