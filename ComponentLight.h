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

	PreComponentReturn PreUpdate() override;
	void Update() override;
	void CleanUp() override;
	void Draw(int id) override;
	void ShowMetadata() override;

	void SetLightType(LightType lightType) { this->lightType = lightType; }
	char* GetLightTypeToString(LightType type);
	void  SetLightTypeFromString(const char* type);

	void Save(System* system) override;
	void Load(System* system, rapidjson::Value& value) override;

public:
	LightType lightType;
	float intensity = 0.4f;

};

#endif // !__ComponentLight_h__