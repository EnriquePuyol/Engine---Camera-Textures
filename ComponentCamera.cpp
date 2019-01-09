#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "ModuleUI.h"

ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent)
{
	type = Camera;

	fboSet.fbo = 0;
	fboSet.fb_depth = 0;
	fboSet.fb_tex = 0;
	fboSet.fb_width = 0;
	fboSet.fb_height = 0;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = parent->transform->GetWorldPosition();
	frustum.front = math::float3(0, 0, -1);
	frustum.up = math::float3(0, 1, 0);
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;

	w = App->window->width;
	h = App->window->height;

	float aspect = (float)w / (float)h;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspect);
}

ComponentCamera::ComponentCamera(ComponentCamera* component)
{
	type = Camera;
	active = component->active;
	owner = component->owner;
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Update()
{
	if (w != App->window->width || h != App->window->height)
		UpdateFrustum();
}

void ComponentCamera::CleanUp()
{

}

void ComponentCamera::Draw(int id)
{
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("CAMERA").x / 2);
	ImGui::Text("CAMERA");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("     X     ").x / 2);
	ImGui::PushID(id);
	if (ImGui::Button("X"))
	{
		Delete();
	}
	ImGui::PopID();

	const char* types[] = { "Primary", "Secondary", "Other" };
	ImGui::Text("Type: ");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##hidelabel", GetCameraTypeToString(cameraType), ImGuiComboFlags_NoArrowButton))
	{
		for (int n = 0; n < ARRAYSIZE(types); n++)
		{
			bool is_selected = (GetCameraTypeToString(cameraType) == types[n]);
			if (ImGui::Selectable(types[n], is_selected))
			{
				if (types[n] == "Primary")
				{
					App->ui->uiGame->primaryCamera = this;
				}
				SetCameraTypeFromString(types[n]);
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
		}
		ImGui::EndCombo();
	}

	ImGui::Spacing();
	float3 globalPos = App->scene->selectedGO->transform->GetWorldPosition();
	ImGui::DragFloat3("Global position", globalPos.ptr(), 0.1f);

}

/*void ComponentCamera::LookAt(const math::float3 & target)
{
	float3 dir = (target - eye).Normalized();
	float3x3 rot = float3x3::LookAt(forward, dir, up, Y_AXIS);
	up = rot * up;
	forward = rot * forward;
	right = rot * right;
}*/

void ComponentCamera::UpdateFrustum()
{
	frustum.pos = owner->transform->GetWorldPosition();

	w = App->window->width;
	h = App->window->height;
	float aspect = (float)w / (float)h;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspect);

	view = frustum.ViewMatrix();
	proj = frustum.ProjectionMatrix();
}

char* ComponentCamera::GetCameraTypeToString(CameraType type)
{
	switch (type)
	{
	case Primary:
		return "Primary";
	case Secondary:
		return "Secondary";
	case Other:
		return "Other";
	}

	return "--none--";
}

void ComponentCamera::SetCameraTypeFromString(const char * type)
{
	if (type == "Primary")
		cameraType = Primary;
	else if (type == "Secondary")
		cameraType = Secondary;
	else if (type == "Other")
		cameraType = Other;
}
