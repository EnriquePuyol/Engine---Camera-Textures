#ifndef __MsTimer_h__
#define __MsTimer_h__

#include "SDL.h"
#include <list>
#include "Application.h"
#include "ModuleUI.h"

class MsTimer
{
public:
	MsTimer()
	{
		timer = 0;
	}
	~MsTimer() {}

	void Start()
	{
		timer = SDL_GetTicks();
	}

	Uint32 Read()
	{
		return timer;
	}

	void Stop()
	{
		Uint32 time = (SDL_GetTicks() - timer);
		//LOG("Timer ended. Result = %lu ms", time);
		App->ui->uiConsole->console.AddLog("Timer ended. Result = %i ms", time);
	}

	void Reset()
	{
		timer = 0;
	}

public:
	Uint32 timer;
};

#endif
