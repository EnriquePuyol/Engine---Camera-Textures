#include "ComponentMaterial.h"



ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(parent)
{
	type = Material;
}

ComponentMaterial::ComponentMaterial(unsigned material)
{
	this->material = material;
}

ComponentMaterial::ComponentMaterial(ComponentMaterial * component)
{
	type = Material;
	active = component->active;
	owner = component->owner;
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::Update()
{

}

void ComponentMaterial::CleanUp()
{

}

void ComponentMaterial::Draw(int id)
{
	ImGui::Spacing();
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("MATERIAL").x / 2);
	ImGui::Text("MATERIAL");

	ImGui::PushID(id);
	if (!defaultMat)
	{
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("     X     ").x / 2);
		if (ImGui::Button("X"))
		{
			Delete();
		}
	}
	ImGui::Text("ToDo: Material");

	ImGui::PopID();
}
