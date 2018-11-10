#ifndef __ModuleCameraEditor_h__
#define __ModuleCameraEditor_h__

#include "Module.h"
#include "Globals.h"

#include "GL/glew.h"
#include "MathGeoLib.h"

class ModuleCameraEditor : public Module
{
public:
	ModuleCameraEditor();
	~ModuleCameraEditor();

	bool            Init();
	update_status	PreUpdate();
	update_status   Update();
	update_status	PostUpdate();
	bool            CleanUp();

	void Move();
	//void DrawCoords();

	void Yaw(float angle);
	void Pitch(float angle);

private:
	void LookAt(const math::float3& eye, const math::float3& target);

public:
	float cameraSpeed = 2.0f;
	float zoomSpeed = 2.0f;
	float sens = 0.03f;

	float4x4 proj = float4x4::identity;
	float4x4 view = float4x4::identity;

private:
	unsigned vbo	  = 0;
	unsigned texture0 = 0;

	GLuint program = GL_FALSE;

	Frustum frustum;
	float aspect = 16 / 9;

	float3 eye	  = { 0.0f, 5.0f, 10.0f };
	//float3 target = { 0.0f, 0.0f, 0.0f };
	float3 forward;
	float3 up;
	float3 right;

	float pitch;

	float4x4 view_matrix;

	//float4x4 tri_model = float4x4::identity;
};

#endif