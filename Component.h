#ifndef __Component_h__
#define __Component_h__

#include "GameObject.h"

enum Type
{
	Transform,
	Camera,
	Mesh,
	Material
};


class Component
{
public:

	Component() {}
	virtual ~Component() {}

	virtual void Enable() {}
	virtual void Disable() {}
	virtual void Update() {}

public:

	Type type;
	bool active;
	GameObject* parent;

};

#endif
