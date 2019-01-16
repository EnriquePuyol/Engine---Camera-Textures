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
class ComponentLight;

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
	void RenderMeshNEW(ComponentMesh * meshComp, ComponentCamera * cameraComp);
	void RenderMeshEditor(ComponentMesh* meshComp, float4x4* view, float4x4* proj);

	void GenerateTexture(FBOset* fboSet);

	void CreateSceneImage();
	void DrawCoords();

public:
	list<ComponentLight*> lights;
	list<ComponentCamera*> cameras;
	list<ComponentMesh*> meshes;

	float4x4 tri_model = float4x4::identity;

	unsigned defaultTexture = 0u;
	// CameraTexture
	unsigned rbo = 0u;

	bool showGrid = true;

	void* context = NULL;
};
