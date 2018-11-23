#ifndef __Component_h__
#define __Component_h__

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

#endif // !__Component_h__
