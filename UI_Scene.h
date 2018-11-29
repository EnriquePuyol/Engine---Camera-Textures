#ifndef __UI_Scene_h__
#define __UI_Scene_h__

#include "UI.h"

class UI_Scene : public UI
{
public:
	UI_Scene(char* name);
	~UI_Scene();

	void Draw() override;

public:
	bool focused = false;
};

#endif // !__UI_Scene_h__