#include "ComponentBBox.h"
#include "Application.h"
#include "ModuleScene.h"
#include "debugdraw.h"

ComponentBBox::ComponentBBox(GameObject* parent) : Component(parent)
{
	type = BBox;

	cornersAABB = new float3[8];
	aabb = new math::AABB();
	aabb->SetNegativeInfinity();

	uID = App->GenerateUUID();
}

ComponentBBox::~ComponentBBox()
{

}

void ComponentBBox::Update()
{
	if (App->scene->selectedGO == owner && owner->enable && aabb->IsFinite()) 
	{
		const ddVec3 boxColor = { 0.12f, 0.5f, 0.12f };
		dd::aabb(aabb->minPoint, aabb->maxPoint, boxColor);
	}
}

void ComponentBBox::UpdateBBox()
{
	if (aabb->IsFinite() && owner->enable) {
		//std::vector<Component*> comps = my_go->GetComponents(MESH);
		//SetAABB((std::vector<ComponentMesh*>*) &comps);
		delete aabb;
		aabb = new math::AABB(minOrigin, maxOrigin);
		aabb->TransformAsAABB(owner->transform->model);
	}

	for (list<GameObject*>::iterator it = owner->childs.begin(); it != owner->childs.end(); ++it)
		(*it)->boundingBox->UpdateBBox();
}

void ComponentBBox::SetAABB(list<Component*> meshes)
{
	aabb->SetNegativeInfinity();

	int totalVertices = 0;

	for (list<Component*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		totalVertices += ((ComponentMesh*)(*it))->meshData.numVertices;

	float3* total = new float3[totalVertices];

	int i = 0;
	for (list<Component*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		for (int k = 0; k < ((ComponentMesh*)(*it))->meshData.numVertices; ++k)
		{
			total[i] = ((ComponentMesh*)(*it))->meshData.vertices[k];
			++i;
		}
	}

	aabb->SetFrom(total, totalVertices);
	aabb->GetCornerPoints(cornersAABB);

	minOrigin = aabb->minPoint;
	maxOrigin = aabb->maxPoint;

	aabb->TransformAsAABB(owner->transform->model);
	delete total;
}
