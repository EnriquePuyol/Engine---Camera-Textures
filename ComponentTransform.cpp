#include "ComponentTransform.h"



ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent)
{
	type = Transform;
	active = true;

	position = { 0.0f, 0.0f, 0.0f };
	rotation = { 0.0f, 0.0f, 0.0f };
	scale	 = { 1.0f, 1.0f, 1.0f };
}


ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Update()
{

}

void ComponentTransform::CleanUp()
{

}

void ComponentTransform::Draw()
{

}
