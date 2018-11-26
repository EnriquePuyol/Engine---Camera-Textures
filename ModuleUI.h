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
#include "UI_Scene.h"
#include "UI_Hierarchy.h"

using namespace std;

class ModuleUI : public Module
{

public:
	ModuleUI();
	~ModuleUI();
	bool Init();
	update_status PreUpdate();
	void Draw();
	update_status PostUpdate();
	bool CleanUp();

	void Events(SDL_Event& event);

	void MainBar();
	void Docking();

private:
	list<UI*> uiWindows;

public:
	UI_About*		uiAbout		  = nullptr;
	UI_Performance* uiPerformance = nullptr;
	UI_Console*		uiConsole	  = nullptr;
	UI_Scene*		uiScene		  = nullptr;
	UI_Hierarchy*	uiHierarchy	  = nullptr;

	//ImGuiIO io;

	bool showInfo = false;
	bool showConsole = true;
	bool showPerformace = false;

};

#endif