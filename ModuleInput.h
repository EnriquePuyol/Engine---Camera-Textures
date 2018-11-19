#ifndef __ModuleInput_h__
#define __ModuleInput_h__

#include "Module.h"
#include "MathGeoLib.h"
#include "SDL/include/SDL_scancode.h"
#include "Math/Point.h"

#define NUM_MOUSE_BUTTONS 5

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	bool GetWindowEvent(EventWindow code) const;

public:
	bool quit = false;

	bool moving = false;
	bool scrolling = false;

	fPoint mouseMotion;
	fPoint mouse;
	int mouseScroll;

private:
	bool		windowEvents[WE_COUNT];
	KeyState*	keyboard = NULL;
	KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];

};

#endif