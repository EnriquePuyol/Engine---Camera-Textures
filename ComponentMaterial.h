#ifndef __ComponentMaterial_h__
#define __ComponentMaterial_h__

#include "Component.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* parent);
	~ComponentMaterial();

	void Update() override;
	void CleanUp() override;
	void Draw(int id) override;

public:

};

#endif // !_ComponentMaterial_h__