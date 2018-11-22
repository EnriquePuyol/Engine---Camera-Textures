#ifndef __GameObject_h__
#define __GameObject_h__

#include <vector>
#include "Component.h"

using namespace std;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();
	Component* AddComponent(Type type)
	{

	}

public:

	char* name;

	GameObject* parent;
	vector<GameObject*> childs;
	vector<Component*> components;
};

#endif