#ifndef __ComponentMesh_h__
#define __ComponentMesh_h__

#include "Component.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* parent);
	ComponentMesh(ComponentMesh* component);
	~ComponentMesh();

	void Update() override;
	void CleanUp() override;
	void Draw(int id) override;
};

#endif // !__ComponentMesh_h__