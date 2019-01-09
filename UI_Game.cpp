#include "UI_Game.h"
#include "Application.h"
#include "ModuleWindow.h"


UI_Game::UI_Game(char* name) : UI(name)
{
	active = true;
}


UI_Game::~UI_Game()
{
	delete primaryCamera;
}

void UI_Game::Draw()
{
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0, 0, 0, 1));
	ImGui::Begin(name, &active);

	if (primaryCamera == nullptr)
	{
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("NO CAMERAS RENDERING...").x / 2);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2);
		ImGui::Text("NO CAMERAS RENDERING...");
	}
	else
	{
		//ImTextureID my_tex_id = primaryCamera->renderedTexture;
		//ImGui::Image((void*)primaryCamera->renderedTexture, ImVec2(App->window->width, App->window->height));
		ImVec2 size = ImGui::GetWindowSize();
		ImGui::SetCursorPos({ -(App->window->width - size.x) / 2,-(App->window->height - size.y) / 2 });

		//ImGui::Image((ImTextureID)primaryCamera->renderedTexture, { (float)App->window->width, (float)App->window->height }, { 0,1 }, { 1,0 });
	}

	ImGui::PopStyleColor();
	ImGui::End();
}
