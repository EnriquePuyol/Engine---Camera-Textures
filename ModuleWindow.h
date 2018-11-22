#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

	void SetResolution(int w = SCREEN_WIDTH, int h = SCREEN_HEIGHT);
	void SetFullScreen();
	void SetBorderless();
	void WindowChanged();

public:

	int width, height;
	bool fullscreen = false;
	bool borderless = false;
	bool resizable = false;
	bool vsync = false;

	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

	SDL_DisplayMode display;
};

#endif // __ModuleWindow_H__