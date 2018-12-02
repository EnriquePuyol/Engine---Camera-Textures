#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"

GameObject::GameObject(const char name[40])
{
	sprintf_s(this->name, name);
}


GameObject::~GameObject()
{

}

void GameObject::Update()
{
	// Update the components
	for (list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->Update();
	}
	// Update the childs
	for (list<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		(*it)->Update();
	}
}

void GameObject::CleanUp()
{

}

Component * GameObject::AddComponent(Type type)
{
	return nullptr;
}

void GameObject::Draw()
{
	ImGuiTreeNodeFlags	flags;

	if (childs.size() == 0)
		flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Leaf;
	else
		flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	ImGui::PushID(this);
	ImGui::PushID(&name);

	bool node_open;

	if (nullptr != App->scene->selectedGO && App->scene->selectedGO == this)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.6f, 0.2f, 1));
		node_open = ImGui::TreeNodeEx(name, flags);

		ImGui::PopStyleColor();
	}
	else
	{
		node_open = ImGui::TreeNodeEx(name, flags);
	}
	

	if (ImGui::IsItemClicked())
	{
		selected = !selected;
		if (selected && this != App->scene->selectedGO)
		{
			App->scene->selectedGO != nullptr ? App->scene->selectedGO->selected = false : 1;
			App->scene->selectedGO = this;
		}
		else if (!this->selected && App->scene->selectedGO == this)
		{
			App->scene->selectedGO = nullptr;
		}
	}
	if (node_open)
	{
		if (childs.size() > 0)
		{
			for (std::list<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
				(*it)->Draw();
		}
		ImGui::TreePop();
	}
	ImGui::PopID();
	ImGui::PopID();
}

void GameObject::DrawComponents()
{
	if (ImGui::Checkbox(" ", &enable)) {}
	ImGui::SameLine();
	ImGui::InputText("Name", name, ARRAYSIZE(name));
	ImGui::Separator();

	for (list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->Draw();
	}
	ImGui::Separator();
}
