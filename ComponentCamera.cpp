#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleScene.h"

ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent)
{
	type = Camera;

	if (App->scene->selectedGO->GetNumComponentsOfType(Transform) == 0)
		App->scene->selectedGO->AddComponent(Transform);
}

ComponentCamera::ComponentCamera(ComponentCamera* component)
{
	type = Camera;
	active = component->active;
	parent = component->parent;
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Update()
{

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
					//ToDo: Eliminar la otra primary
				}
				SetCameraTypeFromString(types[n]);
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
		}
		ImGui::EndCombo();
	}

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
