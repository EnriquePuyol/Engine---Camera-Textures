#ifndef __UI_h__
#define __UI_h__

#include "Globals.h"
#include "imgui/imgui.h"

class UI 
{
public:
	UI(char* name) :name(name) {}
	virtual ~UI() {}

	virtual void Draw() {}

	bool getActive() { return active; }
	char* getName() { return name; }

	void ToggleActive()
	{
		if (active)
			active = false;
		else
			active = true;
	}

public:
	bool active;

protected:
	char* name;

};

#endif // !__UI_h__

