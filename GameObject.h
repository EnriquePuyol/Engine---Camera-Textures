#ifndef __GameObject_h__
#define __GameObject_h__

#include "Component.h"
#include <vector>

using namespace std;

enum NextPreReturn
{
	NONE,
	DELETED,
	COPY,
	CUT,
	PASTE
};

class GameObject
{
public:
	GameObject(const char name[40] = "Empty");
	~GameObject();

	NextPreReturn PreUpdate();
	void Update();
	void CleanUp();
	void Delete();
	Component* AddComponent(Type type);

	void Draw();
	void DrawComponents();

public:

	// General info
	char name[40] = "";
	bool selected = false;
	bool enable;
	NextPreReturn nextPreReturn = NONE;

	// ImGui property
	bool openNode = false;

	// Hierarchy info
	GameObject* parent;
	vector<GameObject*> childs;

	// Inspector info
	vector<Component*> components;

};

#endif // !__GameObject_h__