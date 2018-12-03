#include "ModuleScene.h"
#include "ComponentTransform.h"


ModuleScene::ModuleScene()
{
	// ToDo: Borrar algun dia
	GameObject* helloWorld = new GameObject("Test");
	GameObject* child = new GameObject("Test Child");
	helloWorld->components.push_back(new ComponentTransform(helloWorld));

	helloWorld->childs.push_back(child);
	gameobjects.push_back(helloWorld);
}


ModuleScene::~ModuleScene()
{
}

update_status ModuleScene::Update()
{

	for (list<GameObject*>::iterator it = gameobjects.begin(); it != gameobjects.end(); ++it)
	{
		(*it)->Update();
	}

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	return true;
}

void  ModuleScene::CreateGameObject()
{
	GameObject* myGO = new GameObject();

	if (nullptr == selectedGO)
	{
		gameobjects.push_back(myGO);
	}
	else
	{
		selectedGO->childs.push_back(myGO);
	}

	selectedGO = myGO;
}
