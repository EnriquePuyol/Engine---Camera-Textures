#ifndef __ComponentLight_h__
#define __ComponentLight_h__

#include "Component.h"

enum LightType
{
	Ambient,
	Point,
	Directional,
	SpotLight
};

class ComponentLight : public Component
{
public:
	ComponentLight(GameObject* parent);
	ComponentLight(ComponentLight* component);
	~ComponentLight();

	void Update() override;
	void CleanUp() override;
	void Draw(int id) override;

	void SetLightType(LightType lightType) { this->lightType = lightType; }
	char* GetLightTypeToString(LightType type);
	void  SetLightTypeFromString(const char* type);

public:
	LightType lightType;

};

#endif // !__ComponentLight_h__