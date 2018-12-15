#include "Application.h"
#include "ModuleScene.h"
#include "ComponentTransform.h"


ModuleScene::ModuleScene()
{
	root = new GameObject("Root");
	// ToDo: Borrar algun dia
	GameObject* helloWorld = new GameObject("Test");
	GameObject* child = new GameObject("Test Child");
	helloWorld->components.push_back(new ComponentTransform(helloWorld));

	helloWorld->childs.push_back(child);
	root->childs.push_back(helloWorld);
}


ModuleScene::~ModuleScene()
{
}

update_status ModuleScene::PreUpdate()
{
	if (root->nextPreReturn == GO_PASTE)
	{
		if (nullptr != App->scene->selectedGO)
		{
			root->childs.push_back(new GameObject(App->scene->cutcopyGO, App->scene->selectedGO));
			App->scene->selectedGO->openNode = true;
		}
		else
			root->childs.push_back(new GameObject(App->scene->cutcopyGO, App->scene->root));

		root->nextPreReturn = GO_NONE;
	}

	for (list<GameObject*>::iterator it = root->childs.begin(); it != root->childs.end();)
	{
		if ((*it)->PreUpdate() == GO_DELETED || (*it)->PreUpdate() == GO_CUT)
			root->childs.erase(it++);
		else
			++it;
	}
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update()
{
	for (list<GameObject*>::iterator it = root->childs.begin(); it != root->childs.end(); ++it)
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
		root->childs.push_back(myGO);
	}
	else
	{
		selectedGO->openNode = true;
		selectedGO->selected = !selectedGO->selected;
		selectedGO->childs.push_back(myGO);
	}

	selectedGO = myGO;
	selectedGO->selected = true;
}

void ModuleScene::DeleteGameObject()
{
	selectedGO->nextPreReturn = GO_DELETED;
}
