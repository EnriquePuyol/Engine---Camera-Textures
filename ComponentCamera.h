#ifndef __ComponentCamera_h__
#define __ComponentCamera_h__

#include "Component.h"
#include "GL/glew.h"
#include "MathGeoLib.h"

enum CameraType
{
	Primary,
	Secondary,
	Other
};

struct FBOset {
	unsigned fbo;
	unsigned fb_depth;
	unsigned fb_tex;
	unsigned fb_width;
	unsigned fb_height;
};

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* parent);
	ComponentCamera(ComponentCamera* component);
	~ComponentCamera();

	PreComponentReturn PreUpdate() override;
	void Update() override;
	void CleanUp() override;
	void Draw(int id) override;
	void ShowMetadata() override;

	void LookAt();
	void UpdateFrustum();

	char* GetCameraTypeToString(CameraType type);
	void  SetCameraTypeFromString(const char* type);

	void Save(System* system) override;

public:
	CameraType cameraType = Other;
	int w, h;

	Frustum frustum;
	//GLuint renderedTexture;

	FBOset fboSet;

	float4x4 proj;
	float4x4 view;

	float3 forward;
	float3 up;
	float3 right;
};

#endif // !__ComponentCamera_h__