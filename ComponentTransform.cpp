#include "ComponentTransform.h"
#include "GameObject.h"


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
	owner = component->owner;

	position = component->position;
	rotation = component->rotation;
	scale	 = component->scale;
}

ComponentTransform::~ComponentTransform()
{
	owner = nullptr;
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
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 + ImGui::GetWindowWidth() / 4 );
	ImGui::Checkbox("World", &showGlobal);

	ImGui::Spacing();
	
	if (!showGlobal)
	{
		ImGui::PushID(id);
		ImGui::DragFloat3("Position", position.ptr(), 0.1f);
		ImGui::PopID();

		ImGui::PushID(id);
		ImGui::DragFloat3("Rotation", rotation.ptr(), 0.1f);
		ImGui::PopID();

		ImGui::PushID(id);
		ImGui::DragFloat3("Scale", scale.ptr(), 0.1f);
		ImGui::PopID();
	}
	else
	{
		ImGui::Text("Cannot be edited:");
		ImGui::PushID(id);
		ImGui::DragFloat3("W-Position", GetWorldPosition().ptr(), 0.1f);
		ImGui::PopID();

		ImGui::PushID(id);
		ImGui::DragFloat3("W-Rotation", GetWorldRotation().ptr(), 0.1f);
		ImGui::PopID();

		ImGui::PushID(id);
		ImGui::DragFloat3("W-Scale", GetWorldScale().ptr(), 0.1f);
		ImGui::PopID();
	}

	
}

float3 ComponentTransform::GetWorldPosition()
{	
	if (owner->parent == NULL)
		return position;

	float3 globalTransform = position + owner->parent->transform->GetWorldPosition();
	return globalTransform;
}

float3 ComponentTransform::GetWorldRotation()
{
	if (owner->parent == NULL)
		return rotation;

	float3 globalRotation = rotation + owner->parent->transform->GetWorldRotation();
	return globalRotation;
}

float3 ComponentTransform::GetWorldScale()
{
	if (owner->parent == NULL)
		return scale;

	float3 parent = owner->parent->transform->GetWorldScale();
	float x = scale.x * parent.x;
	float y = scale.y * parent.y;
	float z = scale.z * parent.z;

	float3 globalScale = float3(x, y, z);
	return globalScale;
}
