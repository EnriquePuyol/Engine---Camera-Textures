#include "ModuleScene.h"



ModuleScene::ModuleScene()
{
	GameObject* helloWorld = new GameObject("Test");
	GameObject* child = new GameObject("Test Child");

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

GameObject * ModuleScene::CreateGameObject(bool hasParent)
{
	GameObject* myGO = new GameObject();

	if (!hasParent)
	{
		gameobjects.push_back(myGO);
	}

	return myGO;
}
