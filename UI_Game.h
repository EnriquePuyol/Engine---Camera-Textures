#ifndef __UI_Game_h__
#define __UI_Game_h__

#include "UI.h"

class UI_Game : public UI
{
public:
	UI_Game(char* name);
	~UI_Game();

	void Draw() override;

public:
	bool cameraRendering = false;

};

#endif // !__UI_Game_h__