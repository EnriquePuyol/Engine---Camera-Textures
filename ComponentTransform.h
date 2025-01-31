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

	void CleanUp() override;
	void Draw(int id) override;
	void ShowMetadata() override;

	void Save(System* system) override;
	void Load(System* system, rapidjson::Value& value) override;

	void UpdateTransform();

	float3 GetWorldPosition();
	float3 GetWorldRotation();
	float3 GetWorldScale();

public:
	float3 position;
	float3 eulerRotation;
	float3 scale;

	bool showGlobal = false;

	Quat rotation;
	float4x4 model;

};

#endif // !__ComponentTransform_h__
