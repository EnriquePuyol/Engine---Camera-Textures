#include "UI_Game.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleScene.h"


UI_Game::UI_Game(char* name) : UI(name)
{
	active = true;
}


UI_Game::~UI_Game()
{
	
}

void UI_Game::Draw()
{
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0, 0, 0, 1));
	ImGui::Begin(name, &active);

	if (App->scene->primaryCamera == nullptr)
	{
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("NO CAMERAS RENDERING...").x / 2);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2);
		ImGui::Text("NO CAMERAS RENDERING...");
	}
	else
	{
		ImGui::GetWindowDrawList()->AddImage(
			(void*)App->scene->primaryCamera->fboSet.fb_tex,
			ImVec2(ImGui::GetCursorScreenPos()),
			ImVec2(ImGui::GetCursorScreenPos().x + App->scene->primaryCamera->fboSet.fb_width,
				ImGui::GetCursorScreenPos().y + App->scene->primaryCamera->fboSet.fb_height),
			ImVec2(0, 1), ImVec2(1, 0));
	}

	ImGui::PopStyleColor();
	ImGui::End();
}
