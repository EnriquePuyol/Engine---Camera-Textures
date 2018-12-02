#ifndef __UI_Inspector_h__
#define __UI_Inspector_h__

#include "UI.h"
#include "GameObject.h"

class UI_Inspector : public UI
{
public:
	UI_Inspector(char* name);
	~UI_Inspector();

	void Draw() override;

};

#endif