#include "UI_Scene.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleCameraEditor.h"

UI_Scene::UI_Scene(char* name) : UI(name)
{
	active = true;
}


UI_Scene::~UI_Scene()
{
}

void UI_Scene::Draw()
{
	ImGui::Begin(name, &active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) != KEY_REPEAT &&
		App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) != KEY_REPEAT &&
		App->input->GetMouseButtonDown(SDL_BUTTON_MIDDLE) != KEY_REPEAT)
		focused = ImGui::IsMouseHoveringWindow();

	ImVec2 size = ImGui::GetWindowSize();
	ImVec2 pos = ImGui::GetWindowPos();
	ImGui::SetCursorPos({ -(App->window->width - size.x) / 2,-(App->window->height - size.y) / 2 });

	ImGui::Image((void*)App->camera->fboSet.fb_tex, { (float)App->window->width, (float)App->window->height }, { 0,1 }, { 1,0 });


	if (showSceneOptions)
	{
		ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + size.x, 65), ImColor(99, 152, 216));

		ImGui::SetCursorPos(ImVec2(10, 30 - ImGui::CalcTextSize("Show Grid").y / 2));
		if (ImGui::Button("v"))
			showSceneOptions = false;
		ImGui::SetCursorPos(ImVec2(40, 30 - ImGui::CalcTextSize("Show Grid").y / 2));
		if (ImGui::Checkbox("Show Grid", &App->renderer->showGrid))
		{
			ImGui::Text("Hallo!");
		}
	}
	else
	{
		pos.x += 10;
		ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + 15, 65), ImColor(99, 152, 216));
		ImGui::SetCursorPos(ImVec2(10, 30 - ImGui::CalcTextSize("Show Grid").y / 2));
		if (ImGui::Button("^"))
			showSceneOptions = true;
	}

	ImGui::End();

}
