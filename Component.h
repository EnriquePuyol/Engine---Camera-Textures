#ifndef __Component_h__
#define __Component_h__

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

class GameObject;

enum Type
{
	Transform,
	Mesh,
	Material,
	Camera,
	Light
};

enum PreComponentReturn
{
	COMP_NONE,
	COMP_DELETED,
	COMP_COPY,
	COMP_CUT,
	COMP_PASTE
};

class Component
{

public:
	Component(GameObject* parent) :parent(parent) {}
	virtual ~Component() {}

	virtual void Enable() { active = true; }
	virtual void Disable() { active = false; }

	virtual PreComponentReturn PreUpdate() { return nextPreReturn; }
	virtual void Update() {}
	virtual void CleanUp() {}
	virtual void Draw(int id) {}
	virtual void Delete() { nextPreReturn = COMP_DELETED; }

public:
	Type type;
	bool active;
	GameObject* parent;

	PreComponentReturn nextPreReturn = COMP_NONE;

};

#endif // !__Component_h__
