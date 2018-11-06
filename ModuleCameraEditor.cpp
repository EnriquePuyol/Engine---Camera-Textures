#include "Application.h"

#include "ModuleCameraEditor.h"
#include "ModuleWindow.h"
#include "ModulePrograms.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"

#include "GL/glew.h"
#include "SDL.h"



ModuleCameraEditor::ModuleCameraEditor()
{
}


ModuleCameraEditor::~ModuleCameraEditor()
{
}

bool ModuleCameraEditor::Init()
{
	// https://github.com/d0n3val/Edu-Game-Engine/blob/master/Source/ModuleEditorCamera.cpp

	float vertex_buffer_data[] = {
		// positions of 1st triangle
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		// positions of 2nd triangle
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		 // uvs
		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 0.0f, 1.0f,

		 0.0f, 1.0f,
		 1.0f, 0.0f,
		 1.0f, 1.0f
	};


	// Projection matrix
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f)) * aspect;

	proj = frustum.ProjectionMatrix();

	float3 target(0.0f, 0.0f, 0.0f);
	up = float3(0.0f, 1.0f, 0.0f);
	LookAt(eye, target);

	tri_model = math::float4x4::identity;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	texture0 = App->textures->Load("pikachuFace.png", false);

	return vbo;
}

update_status ModuleCameraEditor::PreUpdate()
{

	return UPDATE_CONTINUE;
}

update_status ModuleCameraEditor::Update()
{
	Move();
	//LookAt(view, eye, target, math::float3(0.0f, 1.0f, 0.0f));
	LookAt(eye, eye + forward);

	// ---------------------
	glUseProgram(App->programs->def_program);

	glUniformMatrix4fv(glGetUniformLocation(App->programs->def_program, "model"), 1, GL_TRUE, &tri_model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programs->def_program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programs->def_program, "proj"), 1, GL_TRUE, &proj[0][0]);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glUniform1i(glGetUniformLocation(App->programs->def_program, "texture0"), 0); // 0 is related to GL_TEXTURE0

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0,                  // attribute 0
		3,                  // number of componentes (3 floats)
		GL_FLOAT,           // data type
		GL_FALSE,           // should be normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)(sizeof(float) * 3 * 6)    // 3 float 6 vertices for jump positions
	);

	DrawCoords();

	glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	return UPDATE_CONTINUE;
}

update_status ModuleCameraEditor::PostUpdate()
{

	return UPDATE_CONTINUE;
}

bool ModuleCameraEditor::CleanUp()
{
	if (vbo != 0)
	{
		glDeleteBuffers(1, &vbo);
	}

	return true;
}

void ModuleCameraEditor::Move()
{
	float speed = cameraSpeed * App->deltaTime;

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);

		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed *= 2;

		// Up & Down
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		{
			eye += up * speed;
		}
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
		{
			eye -= up * speed;
		}
		//Left & Right
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			eye -= right * speed;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			eye += right * speed;
		}
		// Foward & Back
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			eye += forward * speed;
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			eye -= forward * speed;
		}

		// Mouse controls
		if (App->input->moving)
		{
			iPoint mouseMotion = App->input->mouseMotion;
			Pitch(-mouseMotion.y * sens);
			Yaw(-mouseMotion.x * sens);
		}

	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		//SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	if (App->input->scrolling)
	{
		eye += forward * zoomSpeed * App->input->mouseScroll;
	}

}

void ModuleCameraEditor::DrawCoords()
{

	glLineWidth(1.0f);
	//int grid = glGetUniformLocation(program, "newColor");
	//float cream[4] = { 0.988f, 0.918f, 0.592f, 1.0f };
	//glUniform4fv(grid, 1, cream);

	glBegin(GL_LINES);

	float d = 200.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();

	glLineWidth(2.0f);

	// red X
	glBegin(GL_LINES);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
	glEnd();

	// green Y
	glBegin(GL_LINES);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);
	glEnd();

	// blue Z
	glBegin(GL_LINES);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
	glEnd();

	glLineWidth(1.0f);
}

void ModuleCameraEditor::Yaw(float angle)
{
	Quat rotMat = math::Quat::RotateY(angle);
	forward = rotMat * forward;
	up = rotMat * up;
}

void ModuleCameraEditor::Pitch(float angle)
{
	Quat rotMat = Quat::RotateAxisAngle(right, angle * up.AngleBetweenNorm(float3(up.x, 0.f, up.z).Normalized()));
	float3 newForward = rotMat * forward;
	float newPitch = (newForward.AngleBetweenNorm(float3(newForward.x, 0.f, newForward.z).Normalized())) * 0.5f;
	if (newPitch < 1.2f)
	{
		forward = newForward;
		up = rotMat * up;
		pitch = newPitch;
	}
}

void ModuleCameraEditor::LookAt(const math::float3 & eye, const math::float3 & target)
{
	forward = math::float3(target - eye); 
	forward.Normalize();
	right = math::float3(forward.Cross(up)); 
	right.Normalize();
	up = math::float3(right.Cross(forward));

	// axis in rows to do inverse
	view[0][0] = right.x;    view[0][1] = right.y;    view[0][2] = right.z;    view[0][3] = -right.Dot(eye);
	view[1][0] = up.x;       view[1][1] = up.y;       view[1][2] = up.z;       view[1][3] = -up.Dot(eye);
	view[2][0] = -forward.x; view[2][1] = -forward.y; view[2][2] = -forward.z; view[2][3] = forward.Dot(eye);
	view[3][0] = 0;          view[3][1] = 0;          view[3][2] = 0;          view[3][3] = 1;

	pitch = forward.AngleBetweenNorm(float3(forward.x, 0.f, forward.z).Normalized());
}
