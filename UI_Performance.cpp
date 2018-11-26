#include "UI_Performance.h"
#include "Application.h"
#include "ModuleWindow.h"


UI_Performance::UI_Performance(char* name) : UI(name)
{
	//active = false;

	logMSIterator = 0;
	logFPSIterator = 0;
	lastFrameTime = SDL_GetTicks();
	lastSecondTime = SDL_GetTicks();

	fps_log = new float[50];
	ms_log = new float[50];
}


UI_Performance::~UI_Performance()
{
}

void UI_Performance::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(300, App->window->height - 120), ImGuiCond_Always);

	ImGui::Begin(name, &active);

	ImGui::Text("App Lifetime = %d seconds", SDL_GetTicks() / 1000);
	char* title = new char[50];
	UpdateFramerates();
	sprintf_s(title, 50, "Framerate %.1f", fps_log[logFPSIterator]);
	ImGui::PlotHistogram("", fps_log, 50, 0, title, 0.0f, 100.0f, ImVec2(350, 100));
	sprintf_s(title, 50, "Milliseconds %.1f", ms_log[logMSIterator]);
	ImGui::PlotHistogram("", ms_log, 50, 0, title, 0.0f, 100.0f, ImVec2(350, 100));

	ImGui::End();
}

void UI_Performance::UpdateFramerates()
{
	double timeElapsed = SDL_GetTicks() - lastSecondTime;
	//fps calculation
	lastSecondTime = SDL_GetTicks();
	fps_log[logFPSIterator] = 1000 / timeElapsed;
	++logFPSIterator;
	if (logFPSIterator > 49) logFPSIterator = 0;
	//ms calculation
	ms_log[logMSIterator] = timeElapsed;
	lastFrameTime = SDL_GetTicks();
	//iterator increase
	++logMSIterator;
	if (logMSIterator > 49) logMSIterator = 0;
}
