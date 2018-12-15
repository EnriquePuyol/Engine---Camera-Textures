#include "UI_Game.h"



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

	if (!cameraRendering)
	{
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("NO CAMERAS RENDERING...").x / 2);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2);
		ImGui::Text("NO CAMERAS RENDERING...");
	}

	ImGui::PopStyleColor();
	ImGui::End();
}
