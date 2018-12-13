#include "ComponentCamera.h"
#include "Application.h"
#include "ModuleScene.h"

ComponentCamera::ComponentCamera(GameObject* parent) : Component(parent)
{
	type = Camera;

	if (App->scene->selectedGO->GetNumComponentsOfType(Transform) == 0)
		App->scene->selectedGO->AddComponent(Transform);
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
}
