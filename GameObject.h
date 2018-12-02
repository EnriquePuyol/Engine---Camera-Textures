#ifndef __GameObject_h__
#define __GameObject_h__

#include "Component.h"
#include <list>

using namespace std;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();
	void CleanUp();
	Component* AddComponent(Type type);

public:

	string name;

	GameObject* parent;
	list<GameObject*> childs;

	list<Component*> components;

};

#endif // !__GameObject_h__