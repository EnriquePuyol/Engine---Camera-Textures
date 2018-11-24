#ifndef __UI_About_h__
#define __UI_About_h__

#include "UI.h"

class UI_About : public UI
{
public:
	UI_About(char* name);
	~UI_About();

	void Draw();
};

#endif //__UI_About_h__