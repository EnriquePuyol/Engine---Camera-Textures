#include "UI_Scene.h"
#include "Application.h"
#include "ModuleWindow.h"

UI_Scene::UI_Scene(char* name) : UI(name)
{
	//active = true;
}


UI_Scene::~UI_Scene()
{
}

void UI_Scene::Draw()
{
	ImGui::Begin(name, &active);

	ImGui::End();

}
