#ifndef __GameObject_h__
#define __GameObject_h__

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentBBox.h"
#include "System.h"
#include <list>

using namespace std;

enum NextPreReturn
{
	GO_NONE,
	GO_DELETED,
	GO_COPY,
	GO_CUT,
	GO_PASTE
};

class GameObject
{
public:
	GameObject(const char name[40] = "Empty");
	GameObject(GameObject* gameobject, GameObject* parent);
	~GameObject();

	NextPreReturn PreUpdate();
	void Update();
	void CleanUp();
	void Delete();
	void AddComponent(Type type);
	Component* GetComponentOfType(Type type);
	list<Component*> GetAllComponentsOfType(Type type);
	int	 GetNumComponentsOfType(Type type);
	bool CanAddComponentOfType(Type type);

	void Draw();
	void DrawComponents();

	void Save(System* system);

public:

	// General info
	char name[40] = "";
	const char* uID;
	bool selected = false;
	bool enable = true;
	NextPreReturn nextPreReturn = GO_NONE;

	// ImGui property
	bool openNode = false;
	bool showMetadata = false;

	// Hierarchy info
	GameObject* parent = NULL;
	list<GameObject*> childs;

	// Inspector info
	ComponentTransform* transform = NULL;
	ComponentMaterial* material = NULL;
	ComponentBBox* boundingBox = NULL;
	list<Component*> components;

};

#endif // !__GameObject_h__