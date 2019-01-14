#ifndef __ComponentBBox_h__
#define __ComponentBBox_h__

#include "Component.h"
#include "MathGeoLib.h"
#include "ComponentMesh.h"
#include <list>

using namespace std;

class ComponentBBox : public Component
{
public:
	ComponentBBox(GameObject* parent);
	~ComponentBBox();

	void Update() override;
	void UpdateBBox();
	void SetAABB(list<Component*> meshes);

public:
	AABB* aabb;
	float3* cornersAABB;

	float3 minOrigin;
	float3 maxOrigin;
};

#endif