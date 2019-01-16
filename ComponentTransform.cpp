#include "ComponentTransform.h"
#include "GameObject.h"
#include "Application.h"


ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent)
{
	type = Transform;
	active = true;

	position = { 0.0f, 0.0f, 0.0f };
	eulerRotation = { 0.0f, 0.0f, 0.0f };
	scale	 = { 1.0f, 1.0f, 1.0f };

	rotation = Quat::FromEulerXYZ(0.f, 0.f, 0.f);
	model.Set(float4x4::FromTRS(position, rotation, scale));

	uID = App->GenerateUUID();
}

ComponentTransform::ComponentTransform(ComponentTransform * component)
{
	type   = Transform;
	active = component->active;
	owner = component->owner;

	position = component->position;
	eulerRotation = component->eulerRotation;
	scale	 = component->scale;

	rotation = Quat::FromEulerXYZ(eulerRotation.x, eulerRotation.y, eulerRotation.z);
	model.Set(float4x4::FromTRS(position, rotation, scale));

	uID = component->uID;
}

ComponentTransform::~ComponentTransform()
{
	owner = nullptr;
}

void ComponentTransform::CleanUp()
{

}

void ComponentTransform::Draw(int id)
{
	ImGui::PushID(id);

	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("TRANSFORM").x / 2);
	ImGui::Text("TRANSFORM");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 + ImGui::GetWindowWidth() / 4 );
	ImGui::Checkbox("World", &showGlobal);

	ImGui::Spacing();
	
	if (!showGlobal)
	{
		if (ImGui::DragFloat3("Position", position.ptr(), 0.1f))
			UpdateTransform();

		if (ImGui::DragFloat3("Rotation", eulerRotation.ptr(), 0.12f))
			UpdateTransform();
		
		if (ImGui::DragFloat3("Scale", scale.ptr(), 0.01f))
			UpdateTransform();
	}
	else
	{
		ImGui::Text("Cannot be edited:");
		ImGui::DragFloat3("W-Position", GetWorldPosition().ptr(), 0.1f);
		ImGui::DragFloat3("W-Rotation", GetWorldRotation().ptr(), 0.1f);
		ImGui::DragFloat3("W-Scale", GetWorldScale().ptr(), 0.1f);
		
	}

	if (owner->showMetadata)
		ShowMetadata();

	ImGui::PopID();
}

void ComponentTransform::ShowMetadata()
{
	ImGui::SeparatorCustom(50, ImGui::GetWindowWidth() - 100);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4, 0.4, 0.4, 1));
	ImGui::Text("uID:");
	ImGui::Text(uID);
	ImGui::PopStyleColor();
}

void ComponentTransform::Save(System * system)
{
	system->StartObject();

	system->AddComponentType("compType", type);
	system->AddString("ownerUID", owner->uID);
	system->AddString("uID", uID);

	system->AddFloat3("position", position);
	system->AddFloat3("eulerRotation", eulerRotation);
	system->AddFloat3("scale", scale);
	system->AddQuat("rotation", rotation);

	system->EndObject();
}

void ComponentTransform::Load(System * system, rapidjson::Value & value)
{
	sprintf(uID, system->GetString("uID", value));

	position = system->GetFloat3("position", value);
	eulerRotation = system->GetFloat3("eulerRotation", value);
	scale = system->GetFloat3("scale", value);
	rotation = system->GetQuat("rotation", value);

	UpdateTransform();
}

void ComponentTransform::UpdateTransform()
{
	rotation = Quat::FromEulerXYZ(math::DegToRad(eulerRotation.x), math::DegToRad(eulerRotation.y) , math::DegToRad(eulerRotation.z));

	model.Set(float4x4::FromTRS(position, rotation, scale));

	if (owner->parent != nullptr)
		model = owner->parent->transform->model * model;

	owner->boundingBox->UpdateBBox();

	for (list<GameObject*>::iterator it = owner->childs.begin(); it != owner->childs.end(); ++it)
		(*it)->transform->UpdateTransform();
}

float3 ComponentTransform::GetWorldPosition()
{	
	if (owner == NULL)
		LOG("fefofbno2");

	if (owner->parent == NULL)
		return position;

	float3 globalTransform = position + owner->parent->transform->GetWorldPosition();
	return globalTransform;
}

float3 ComponentTransform::GetWorldRotation()
{
	if (owner->parent == NULL)
		return eulerRotation;

	float3 globalRotation = eulerRotation + owner->parent->transform->GetWorldRotation();
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
