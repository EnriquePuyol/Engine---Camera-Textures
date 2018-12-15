#ifndef __ComponentCamera_h__
#define __ComponentCamera_h__

#include "Component.h"

enum CameraType
{
	Primary,
	Secondary,
	Other
};

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* parent);
	ComponentCamera(ComponentCamera* component);
	~ComponentCamera();

	void Update() override;
	void CleanUp() override;
	void Draw(int id) override;

	char* GetCameraTypeToString(CameraType type);
	void  SetCameraTypeFromString(const char* type);

public:
	CameraType cameraType = Other;
};

#endif // !__ComponentCamera_h__