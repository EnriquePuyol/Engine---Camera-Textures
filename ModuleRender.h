#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"
#include "GL/glew.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init() override;
	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);

	void CreateSceneImage();
	void DrawCoords();

public:
	float4x4 tri_model = float4x4::identity;

	GLuint renderedTexture = 0;
	unsigned fbo = 0u;
	unsigned rbo = 0u;

	void* context = NULL;
};
