#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModulePrograms;
class ModuleRenderExercise;
class ModuleCameraEditor;
class ModuleModelLoader;
class ModuleUI;
class MsTimer;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void Tick();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleInput* input = nullptr;
	ModulePrograms* programs = nullptr;
    ModuleRenderExercise* exercise = nullptr;
	ModuleCameraEditor* camera = nullptr;
	ModuleModelLoader* modelLoader = nullptr;
	ModuleUI* ui = nullptr;

	MsTimer* timer;

public:
	float prevTicks;
	float deltaTime;
	float auxTimer;

	int frameCount;
	int fps;

private:

	std::list<Module*> modules;

};

extern Application* App;
