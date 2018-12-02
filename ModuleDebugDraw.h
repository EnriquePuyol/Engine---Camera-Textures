#ifndef _MODULE_DEBUGDRAW_H_
#define _MODULE_DEBUGDRAW_H_

#include "Module.h"

class DDRenderInterfaceCoreGL;

class ModuleDebugDraw : public Module
{

public:

	ModuleDebugDraw();
	~ModuleDebugDraw();

	bool            Init() override;
	update_status   Update() override;
	bool            CleanUp() override;

	void            Draw(unsigned fbo, unsigned fb_width, unsigned fb_height);
private:

	static DDRenderInterfaceCoreGL* implementation;
};

#endif /* _MODULE_DEBUGDRAW_H_ */
