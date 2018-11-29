#include "UI_Inspector.h"



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

	if (nullptr == selectedGameObject)
	{
		ImGui::Text("No gameobject selected...");
	}
	else
	{

	}

	ImGui::End();
}
