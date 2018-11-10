#ifndef __ModuleUI_h__
#define __ModuleUI_h__

#include "Module.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SDL.h"

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

public:
	ImGuiIO io;
};

#endif