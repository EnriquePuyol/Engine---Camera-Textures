#ifndef __ComponentMaterial_h__
#define __ComponentMaterial_h__

#include "Component.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* parent);
	ComponentMaterial(unsigned material);
	ComponentMaterial(ComponentMaterial* component);
	~ComponentMaterial();

	void Draw(int id) override;

public:
	unsigned material;
	bool defaultMat = false;
};

#endif // !_ComponentMaterial_h__