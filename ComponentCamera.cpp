#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleUI.h"
#include "debugdraw.h"

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

	forward = float3(0.0f, 0.0f, 1.0f);
	up = float3(0.0f, 1.0f, 0.0f);
	right = float3(1.0f, 0.0f, 0.0f);

	App->renderer->cameras.push_back(this);

	uID = App->GenerateUUID();
}

ComponentCamera::ComponentCamera(ComponentCamera* component)
{
	type = Camera;
	active = component->active;
	owner = component->owner;

	App->renderer->cameras.push_back(this);

	uID = component->uID;
}

ComponentCamera::~ComponentCamera()
{
}

PreComponentReturn ComponentCamera::PreUpdate()
{
	// Si borramos el componente quitamos la camara del Render
	if (nextPreReturn == COMP_DELETED)
	{
		for (list<ComponentCamera*>::iterator it = App->renderer->cameras.begin(); it != App->renderer->cameras.end(); )
		{
			if ((*it) != this)
				++it;
			else
				App->renderer->cameras.erase(it++);
		}
	}

	return nextPreReturn;
}

void ComponentCamera::Update()
{
	UpdateFrustum();

	const ddVec3 boxColor = { 0.25f, 0.77f, 0.95f };
	dd::frustum((proj * view).Inverted(), boxColor);
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
					if(App->scene->primaryCamera != NULL)
						App->scene->primaryCamera->cameraType = Other;
					App->scene->primaryCamera = this;
				}
				SetCameraTypeFromString(types[n]);
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
		}
		ImGui::EndCombo();
	}
	ShowMetadata();
}

void ComponentCamera::ShowMetadata()
{
	if (owner->showMetadata)
	{
		ImGui::SeparatorCustom(50, ImGui::GetWindowWidth() - 100);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4, 0.4, 0.4, 1));
		ImGui::Text("uID:");
		ImGui::Text(uID);
		ImGui::PopStyleColor();
	}
}

void ComponentCamera::LookAt()
{
	frustum.front = (owner->transform->rotation * WORLD_FRONT).Normalized();
	frustum.up = (owner->transform->rotation * WORLD_UP).Normalized();
}

void ComponentCamera::UpdateFrustum()
{
	frustum.pos = owner->transform->GetWorldPosition();
	LookAt();

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

void ComponentCamera::Save(System * system)
{
	system->StartObject();

	system->AddComponentType("compType", type);
	system->AddString("ownerUID", owner->uID);
	system->AddString("uID", uID);
	system->AddInt("cameraType", cameraType);

	system->AddFloat("frustum.nearPlaneDistance", frustum.nearPlaneDistance);
	system->AddFloat("frustum.farPlaneDistance", frustum.farPlaneDistance);
	system->AddFloat3("frustum.pos", frustum.pos);
	system->AddFloat3("frustum.front", frustum.front);
	system->AddFloat3("frustum.up", frustum.up);

	system->EndObject();
}
