#include "ComponentTransform.h"



ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent)
{
	type = Transform;
	active = true;

	position = { 0.0f, 0.0f, 0.0f };
	rotation = { 0.0f, 0.0f, 0.0f };
	scale	 = { 1.0f, 1.0f, 1.0f };
}


ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Update()
{

}

void ComponentTransform::CleanUp()
{

}

void ComponentTransform::Draw()
{
	
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("TRANSFORM").x / 2);
	ImGui::Text("TRANSFORM");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("     X     ").x / 2);
	if (ImGui::Button("X"))
	{

	}
	ImGui::Spacing();
	
	ImGui::DragFloat3("Position", position.ptr(), 0.1f);
	ImGui::DragFloat3("Rotation", rotation.ptr(), 0.1f);
	ImGui::DragFloat3("Scale",	  scale.ptr(),    0.1f);

	
}

void ComponentTransform::Delete()
{
	delete this;
}
