#include "GameObject.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleScene.h"
#include "UI_Hierarchy.h"
#include "ComponentTransform.h"

GameObject::GameObject(const char name[40])
{
	sprintf_s(this->name, name);
}


GameObject::~GameObject()
{

}

NextPreReturn GameObject::PreUpdate()
{

	for (list<Component*>::iterator it = components.begin(); it != components.end();)
	{
		if ((*it)->PreUpdate() == COMP_DELETED)
			components.erase(it++);
		else
			++it;
	}

	for (list<GameObject*>::iterator it = childs.begin(); it != childs.end();)
	{
		
		if ((*it)->PreUpdate() != GO_DELETED)
			++it;
		else
			childs.erase(it++);
		
	}

	if (nextPreReturn == GO_DELETED)
	{
		Delete();
		return GO_DELETED;
	}
	return GO_NONE;
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

void GameObject::Delete()
{

	for (auto &component : components)
	{
		RELEASE(component);
	}

	for (auto &child : childs)
	{
		RELEASE(child);
	}

	App->scene->selectedGO = nullptr;
}

void GameObject::AddComponent(Type type)
{
	if (type == Transform)
	{
		components.push_back(new ComponentTransform(this));
	}
}

int GameObject::GetNumComponentsOfType(Type type)
{
	int contador = 0;

	for (list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->type == type)
			contador++;
	}

	return contador;
}

void GameObject::Draw()
{
	if (openNode)
	{
		ImGui::SetNextTreeNodeOpen(true);
		openNode = false;
	}

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
	// When is clicked but no when the arrow is clicked
	if (ImGui::IsItemClicked() || 
		ImGui::IsItemClicked(1) && nullptr == App->scene->selectedGO ||
		ImGui::IsItemClicked(1) && this != App->scene->selectedGO)
	{
		App->ui->uiHierarchy->isItemClicked = true;

		if ((ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing())
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
	}
	if (node_open)
	{
		if (childs.size() > 0)
		{
			for (list<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
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
	ImGui::Spacing();
	ImGui::Separator();

	int i = 0;
	for (list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->Draw(i);
		ImGui::Separator();
		i++;
	}
}
