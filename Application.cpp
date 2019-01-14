#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModulePrograms.h"
#include "ModuleCameraEditor.h"
#include "ModuleModelLoader.h"
#include "ModuleDebugDraw.h"
#include "ModuleUI.h"
#include "ModuleScene.h"
#include "MsTimer.h"
#include "guid.hpp"

using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(window	  = new ModuleWindow());
	modules.push_back(renderer	  = new ModuleRender());
	modules.push_back(textures	  = new ModuleTextures());
	modules.push_back(input		  = new ModuleInput());
	modules.push_back(ui		  = new ModuleUI());
    modules.push_back(programs	  = new ModulePrograms());
	modules.push_back(camera	  = new ModuleCameraEditor());
	modules.push_back(modelLoader = new ModuleModelLoader());
	modules.push_back(scene		  = new ModuleScene());
	modules.push_back(debugDraw	  = new ModuleDebugDraw());
	timer = new MsTimer();
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	prevTicks  = 0;
	deltaTime  = 0;
	auxTimer   = 0;
	frameCount = 0;
	fps		   = 0;

	bool ret = true;

	timer->Start();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	timer->Stop();

	App->ui->uiConsole->console.AddLog("--- Welcome to SAG Engine! ---");

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	Tick();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}

void Application::Tick()
{
	frameCount++;
	Uint32 ticksNow = SDL_GetTicks();
	deltaTime = (ticksNow - prevTicks) / 1000;
	auxTimer += deltaTime;
	prevTicks = ticksNow;
	if (auxTimer >= 1)
	{
		fps = frameCount;
		frameCount = 0;
		auxTimer = 0;
	}
}

const char* Application::GenerateUUID()
{
	string uuid = xg::newGuid().str();
	char* newUUID = new char[strlen(uuid.c_str())];
	strcpy(newUUID, uuid.c_str());
	return newUUID;
}
