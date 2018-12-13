#ifndef __ComponentCamera_h__
#define __ComponentCamera_h__

#include "Component.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* parent);
	~ComponentCamera();

	void Update() override;
	void CleanUp() override;
	void Draw(int id) override;
};

#endif // !__ComponentCamera_h__