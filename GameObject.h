#ifndef __GameObject_h__
#define __GameObject_h__

#include "Component.h"
#include <list>

using namespace std;

class GameObject
{
public:
	GameObject(const char name[40] = "Empty");
	~GameObject();

	void Update();
	void CleanUp();
	Component* AddComponent(Type type);

	void Draw();
	void DrawComponents();

public:

	char name[40] = "";
	bool selected = false;
	bool enable;

	GameObject* parent;
	list<GameObject*> childs;

	list<Component*> components;

};

#endif // !__GameObject_h__