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
class ModuleDebugDraw;
class ModuleUI;
class ModuleScene;
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
	const char* GenerateUUID();

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
	ModuleScene* scene = nullptr;
	ModuleDebugDraw* debugDraw = nullptr;

	MsTimer* timer = nullptr;

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
