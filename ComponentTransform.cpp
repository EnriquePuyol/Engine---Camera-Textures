#include "ComponentTransform.h"



ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent)
{
	type = Transform;
	active = true;

	position = { 0.0f, 0.0f, 0.0f };
	rotation = { 0.0f, 0.0f, 0.0f };
	scale	 = { 1.0f, 1.0f, 1.0f };
}

ComponentTransform::ComponentTransform(ComponentTransform * component)
{
	type   = Transform;
	active = component->active;
	parent = component->parent;

	position = component->position;
	rotation = component->rotation;
	scale	 = component->scale;
}

ComponentTransform::~ComponentTransform()
{
	parent = nullptr;
}

void ComponentTransform::Update()
{

}

void ComponentTransform::CleanUp()
{

}

void ComponentTransform::Draw(int id)
{
	
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("TRANSFORM").x / 2);
	ImGui::Text("TRANSFORM");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("     X     ").x / 2);
	ImGui::PushID(id);
	if (ImGui::Button("X"))
	{
		Delete();
	}
	ImGui::PopID();

	ImGui::Spacing();
	
	ImGui::PushID(id);
	ImGui::DragFloat3("Position", position.ptr(), 0.1f);
	ImGui::PopID();
	
	ImGui::PushID(id); 
	ImGui::DragFloat3("Rotation", rotation.ptr(), 0.1f);
	ImGui::PopID();

	ImGui::PushID(id);
	ImGui::DragFloat3("Scale",	  scale.ptr(),    0.1f);
	ImGui::PopID();

	
}
