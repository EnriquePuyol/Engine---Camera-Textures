#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"
#include "ComponentCamera.h"
#include "GL/glew.h"
#include <list>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ComponentCamera;
class ComponentMesh;

using namespace std;

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

	void RenderEditorCamera();
	void RenderGameCameras();
	void RenderMesh(ComponentMesh * meshComp, ComponentCamera * cameraComp);

	void GenerateTexture(FBOset* fboSet);

	void CreateSceneImage();
	void DrawCoords();

public:
	list<ComponentCamera*> cameras;
	list<ComponentMesh*> meshes;

	float4x4 tri_model = float4x4::identity;

	GLuint renderedTexture = 0; // ToDo: Borrar si no se usa
	unsigned fbo = 0u; // ToDo: Borrar si no se usa
	unsigned rbo = 0u; // ToDo: Borrar si no se usa

	bool showGrid = true;

	void* context = NULL;
};
