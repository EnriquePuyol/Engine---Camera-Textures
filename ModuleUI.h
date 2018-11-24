#ifndef __ModuleUI_h__
#define __ModuleUI_h__

#include "Module.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SDL.h"

#include <list>
#include "UI.h"
#include "UI_About.h"
#include "UI_Performance.h"
#include "UI_Console.h"

using namespace std;

class ModuleUI : public Module
{

public:
	ModuleUI();
	~ModuleUI();
	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void Events(SDL_Event& event);

	void MainBar();

private:
	list<UI*> uiWindows;

public:
	UI_About* uiAbout = nullptr;
	UI_Performance* uiPerformance = nullptr;
	UI_Console* uiConsole = nullptr;

	ImGuiIO io;

	bool showInfo = false;
	bool showConsole = true;
	bool showPerformace = false;

};

#endif