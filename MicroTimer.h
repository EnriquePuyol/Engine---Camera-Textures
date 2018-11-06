#ifndef __MicroTimer_h__
#define __MicroTimer_h__

#include "SDL.h"
#include <list>

class MicroTimer
{
	MicroTimer() 
	{
		timer = 0;
	}
	~MicroTimer() {}

	void Start()
	{
		timer = SDL_GetPerformanceCounter();
	}

	Uint64 Read()
	{
		return timer;
	}

	void Stop()
	{
		Uint64 time = (SDL_GetPerformanceCounter() - timer) * 1000 / frequency;
	}

	void Reset()
	{
		timer = 0;
	}

public:
	Uint64 timer;

private:
	static Uint64 frequency;

};

Uint64 MicroTimer::frequency = SDL_GetPerformanceFrequency();

#endif // !__MicroTimer_h__

