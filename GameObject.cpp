#include "GameObject.h"

GameObject::GameObject()
{

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
