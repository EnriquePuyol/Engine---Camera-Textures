#include "UI_Scene.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"

UI_Scene::UI_Scene(char* name) : UI(name)
{
	//active = true;
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
	ImGui::SetCursorPos({ -(App->window->width - size.x) / 2,-(App->window->height - size.y) / 2 });

	ImGui::Image((ImTextureID)App->renderer->renderedTexture, { (float)App->window->width, (float)App->window->height }, { 0,1 }, { 1,0 });

	ImGui::End();

}
