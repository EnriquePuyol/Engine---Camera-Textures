#ifndef __ModuleCameraEditor_h__
#define __ModuleCameraEditor_h__

#include "Module.h"
#include "Globals.h"
#include "ComponentCamera.h"

#include "GL/glew.h"
#include "MathGeoLib.h"

class ModuleCameraEditor : public Module
{
public:
	ModuleCameraEditor();
	~ModuleCameraEditor();

	bool            Init() override;
	update_status   Update() override;
	update_status	PostUpdate() override;

	void UpdateFoV(unsigned w, unsigned h);
	void UpdateFrustum();

	void Move();

	void Yaw(float angle);
	void Pitch(float angle);
	void LookAt(const math::float3& target);

public:
	float cameraSpeed = 2.0f;
	float zoomSpeed = 2.0f;
	float sens = 50.0f;
	float smooth = 0.005f;

	FBOset fboSet;
	Frustum frustum;

	float4x4 proj = float4x4::identity;
	float4x4 view = float4x4::identity;

private:

	GLuint program = GL_FALSE;

	//float aspect = 16 / 9;

	float3 eye = { 0.0f, 5.0f, 10.0f };
	float3 forward;
	float3 up;
	float3 right;

	float pitch;
};

#endif