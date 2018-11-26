#include "UI_About.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleUI.h"



UI_About::UI_About(char* name) : UI(name)
{
	active = false;
}


UI_About::~UI_About()
{
}

void UI_About::Draw()
{
	ImGui::Begin(name, &active, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize);

	ImGui::Text(TITLE);
	ImGui::NewLine();
	ImGui::Text("Version: 1.0");
	ImGui::Text("3D game engine developed by Enrique Puyol Martín");
	ImGui::Text("Libraries: ");
	ImGui::Text("	- glew-2.1.0");
	ImGui::Text("	- Imgui");
	ImGui::Text("	- DevIL");
	ImGui::Text("	- OpenGL 3.0");
	ImGui::Text("	- MathGeoLib");
	ImGui::Text("	- SDL 2.0");
	ImGui::Text("	- Brofiler 1.1.2");
	ImGui::Text("MIT License Copyright (c) [2018] [Enrique Puyol]");

	ImGui::End();
}
