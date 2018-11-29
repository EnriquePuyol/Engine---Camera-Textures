#include "UI_Console.h"

UI_Console::UI_Console(char* name) : UI(name)
{
	active = true;
}


UI_Console::~UI_Console()
{
}

void UI_Console::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(App->window->width, 100));
	console.Draw(name);
	
}
