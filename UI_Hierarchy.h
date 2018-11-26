#ifndef __UI_Hierarchy_h__
#define __UI_Hierarchy_h__

#include "UI.h"

class UI_Hierarchy : public UI
{
public:
	UI_Hierarchy(char* name);
	~UI_Hierarchy();

	void Draw() override;
};

#endif // !__UI_Hierarchy_h__