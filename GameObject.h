#ifndef __GameObject_h__
#define __GameObject_h__

#include "Component.h"
#include <vector>

using namespace std;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();
	Component* AddComponent(Type type);

public:

	string name;

	GameObject* parent;
	vector<GameObject*> childs;

	vector<Component*> components;

};

#endif // !__GameObject_h__