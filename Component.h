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
	Material
};

class Component
{

public:
	Component(GameObject* parent) :parent(parent) {}
	virtual ~Component() {}

	virtual void Enable() { active = true; }
	virtual void Disable() { active = false; }

	virtual void Update() {}
	virtual void CleanUp() {}
	virtual void Draw() {}

public:
	Type type;
	bool active;
	GameObject* parent;

};

#endif // !__Component_h__
