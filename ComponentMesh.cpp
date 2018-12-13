#include "ComponentMesh.h"



ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	type = Mesh;
}


ComponentMesh::~ComponentMesh()
{
}

void ComponentMesh::Update()
{

}

void ComponentMesh::CleanUp()
{

}

void ComponentMesh::Draw(int id)
{
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("MESH").x / 2);
	ImGui::Text("MESH");
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("     X     ").x / 2);
	ImGui::PushID(id);
	if (ImGui::Button("X"))
	{
		Delete();
	}
	ImGui::PopID();
}
