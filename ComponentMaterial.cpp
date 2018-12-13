#include "ComponentMaterial.h"



ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(parent)
{
	type = Material;
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
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("MATERIAL").x / 2);
	ImGui::Text("MATERIAL");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("     X     ").x / 2);
	ImGui::PushID(id);
	if (ImGui::Button("X"))
	{
		Delete();
	}
	ImGui::PopID();
}
