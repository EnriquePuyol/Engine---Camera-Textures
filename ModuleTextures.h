#pragma once
#include<list>
#include "Module.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init() override;
	bool CleanUp() override;

	unsigned Load(const char* path, bool mipmaps);
	void Unload(unsigned id);

};