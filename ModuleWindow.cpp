#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleUI.h"

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	//App->ui->uiConsole->console.AddLog("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		//App->ui->uiConsole->console.AddLog("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		SDL_GetCurrentDisplayMode(0, &display);
		display.w = SCREEN_WIDTH; 
		display.h = SCREEN_HEIGHT;
		width = display.w;
		height = display.h;

		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

		if(FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
			fullscreen = true;
		}
		if (BORDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
			borderless = true;
		}
		if (RESIZABLE)
		{
			flags |= SDL_WINDOW_RESIZABLE;
			resizable = true;
		}
		if (VSYNC)
		{
			SDL_GL_SetSwapInterval(vsync);
			vsync = true;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			//App->ui->uiConsole->console.AddLog("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	//LOG("Destroying SDL window and quitting all SDL systems");
	App->ui->uiConsole->console.AddLog("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetResolution(int w, int h)
{
	//fullscreen = fs;

	display.w = w;
	display.h = h;

	width = w;
	height = h;

	SDL_SetWindowSize(window, width, height);
	App->renderer->WindowResized(width, height);
}

void ModuleWindow::SetFullScreen()
{

	Uint32 fullscreenFlag = SDL_WINDOW_FULLSCREEN;
	bool isFullscreen = SDL_GetWindowFlags(window) & fullscreenFlag;
	SDL_SetWindowFullscreen(window, isFullscreen ? 0 : fullscreenFlag);
	SDL_ShowCursor(isFullscreen);

	SDL_GetCurrentDisplayMode(0, &display);

	SetResolution(display.w, display.h);

}

void ModuleWindow::SetBorderless()
{

	SDL_bool isBorderless = SDL_TRUE;

	if (borderless)
		isBorderless = SDL_FALSE;

	SDL_SetWindowBordered(window, isBorderless);
}

void ModuleWindow::WindowChanged()
{
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);

	SetResolution(DM.w, DM.h);
}

