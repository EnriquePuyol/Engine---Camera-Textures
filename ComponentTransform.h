#ifndef __ComponentTransform_h__
#define __ComponentTransform_h__

#include "Component.h"
#include "MathGeoLib.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* parent);
	ComponentTransform(ComponentTransform* component);
	~ComponentTransform();

	void Update() override;
	void CleanUp() override;
	void Draw(int id) override;
	float3 GetWorldPosition();
	float3 GetWorldRotation();
	float3 GetWorldScale();

public:
	float3 position;
	float3 rotation;
	float3 scale;

	bool showGlobal = false;
};

#endif // !__ComponentTransform_h__
