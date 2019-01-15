#include "Application.h"

#include "ModuleCameraEditor.h"
#include "ModuleWindow.h"
#include "ModulePrograms.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleUI.h"
#include "UI_Scene.h"

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
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	float aspect = App->window->width / App->window->height;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f)) * aspect;

	proj = frustum.ProjectionMatrix();

	float3 target(0.0f, 0.0f, 0.0f);
	up = float3(0.0f, 1.0f, 0.0f);

	LookAt(target);

	App->window->SetResolution();

	return true;
}

update_status ModuleCameraEditor::Update()
{
	if (App->ui->uiScene->focused)
		Move();
	return UPDATE_CONTINUE;
}

update_status ModuleCameraEditor::PostUpdate()
{
	LookAt(eye + forward);
	return UPDATE_CONTINUE;
}

void ModuleCameraEditor::UpdateFoV(unsigned w, unsigned h)
{
	float aspect = (float)w / (float)h;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect);
	UpdateFrustum();
}

void ModuleCameraEditor::UpdateFrustum()
{
	frustum.pos = eye;
	frustum.front = forward.Normalized();
	frustum.up = up.Normalized();


	view = frustum.ViewMatrix();
	proj = frustum.ProjectionMatrix();
}

void ModuleCameraEditor::Move()
{
	float speed = cameraSpeed * App->deltaTime;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed *= 2;

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);

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
			fPoint mouseMotion = App->input->mouseMotion;
			Pitch(-mouseMotion.y * smooth);
			Yaw(-mouseMotion.x * smooth);
		}

	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		if (App->input->moving)
		{
			fPoint mov = App->input->mouseMotion;
			eye += (-right * speed * mov.x) + (up * speed * mov.y);
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		up = float3(0.0f, 1.0f, 0.0f);
		LookAt(App->scene->selectedGO->transform->GetWorldPosition());

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			Quat yawRotMat = Quat::RotateY(-speed);
			eye = yawRotMat * eye;
			forward = yawRotMat * forward;
			up = yawRotMat * up;
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			Quat yawRotMat = Quat::RotateY(speed);
			eye = yawRotMat * eye;
			forward = yawRotMat * forward;
			up = yawRotMat * up;
		}
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

void ModuleCameraEditor::Yaw(float angle)
{
	Quat rotMat = math::Quat::RotateY(angle * sens / 100.0f);
	forward = rotMat * forward;
	up = rotMat * up;
}

void ModuleCameraEditor::Pitch(float angle)
{
	Quat rotMat = Quat::RotateAxisAngle(right, angle * sens / 100.0f * up.AngleBetweenNorm(float3(up.x, 0.f, up.z).Normalized()));
	float3 newForward = rotMat * forward;
	float newPitch = (newForward.AngleBetweenNorm(float3(newForward.x, 0.f, newForward.z).Normalized()));
	if (newPitch < 1.2f)
	{
		forward = newForward;
		up = rotMat * up;
		pitch = newPitch;
	}
}

void ModuleCameraEditor::LookAt(const math::float3 & target)
{
	forward = math::float3(target - eye);
	forward.Normalize();
	right = math::float3(forward.Cross(up));
	right.Normalize();
	up = math::float3(right.Cross(forward));

	view[0][0] = right.x;    view[0][1] = right.y;    view[0][2] = right.z;    view[0][3] = -right.Dot(eye);
	view[1][0] = up.x;       view[1][1] = up.y;       view[1][2] = up.z;       view[1][3] = -up.Dot(eye);
	view[2][0] = -forward.x; view[2][1] = -forward.y; view[2][2] = -forward.z; view[2][3] = forward.Dot(eye);
	view[3][0] = 0;          view[3][1] = 0;          view[3][2] = 0;          view[3][3] = 1;

	pitch = forward.AngleBetweenNorm(float3(forward.x, 0.f, forward.z).Normalized());
}
