#include "Application.h"
#include "System.h"
#include "ModuleScene.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentLight.h"
#include "ModuleUI.h"
#include "ModuleRender.h"


ModuleScene::ModuleScene()
{
	root = new GameObject("Root");
	//root->transform = new ComponentTransform(root);
}


ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	// Default scene
	GameObject* house = new GameObject("House");
	GameObject* camera = new GameObject("Main camera");
	// lightGO = new GameObject("Light");
	GameObject* light = new GameObject("Light");

	root->childs.push_back(house);
	root->childs.push_back(camera);
	root->childs.push_back(light);

	house->parent = root;
	camera->parent = root;
	light->parent = root;

	house->components.push_back(new ComponentMesh(house, "BakerHouse.fbx"));
	((ComponentMesh*)house->GetComponentOfType(Mesh))->LoadMesh("BakerHouse.fbx");

	//ComponentCamera
	camera->transform->position = float3(3.0f, 1.5f, 9.5f);
	camera->components.push_back(new ComponentCamera(camera));
	((ComponentCamera*)camera->GetComponentOfType(Camera))->cameraType = Primary;
	App->scene->primaryCamera = ((ComponentCamera*)camera->GetComponentOfType(Camera));

	light->components.push_back(new ComponentLight(light));

	return true;
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
		{
			root->childs.push_back(new GameObject(App->scene->cutcopyGO, App->scene->root));
		}

		root->nextPreReturn = GO_NONE;
	}

	for (list<GameObject*>::iterator it = root->childs.begin(); it != root->childs.end();)
	{
		if ((*it)->PreUpdate() == GO_CUT || (*it)->PreUpdate() == GO_DELETED)
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
		myGO->parent = root;
	}
	else
	{
		selectedGO->openNode = true;
		selectedGO->selected = !selectedGO->selected;
		selectedGO->childs.push_back(myGO);
		myGO->parent = selectedGO;
	}

	selectedGO = myGO;
	selectedGO->selected = true;
}

GameObject * ModuleScene::GetGameObjectByUUID(GameObject * gameObject, char uuidObjectName[37])
{
	GameObject* result = nullptr;

	if (result == nullptr && (strcmp(gameObject->uID, uuidObjectName) == 0)) {
		result = gameObject;
	}
	else {
		for (auto &child : gameObject->childs) {
			if (child->childs.size() > 0) {
				result = GetGameObjectByUUID(child, uuidObjectName);
			}

			if (result == nullptr && (strcmp(child->uID, uuidObjectName) == 0)) {
				result = child;
				break;
			}
			else if (result != nullptr) {
				break;
			}
		}
	}

	return result;
}

void ModuleScene::DeleteGameObject()
{
	selectedGO->nextPreReturn = GO_DELETED;

	for (list<GameObject*>::iterator it = selectedGO->childs.begin(); it != selectedGO->childs.end(); ++it)
	{
		(*it)->nextPreReturn = GO_DELETED;
	}

	for (list<Component*>::iterator it = selectedGO->components.begin(); it != selectedGO->components.end(); ++it)
	{
		(*it)->nextPreReturn = COMP_DELETED;
	}
}

void ModuleScene::ResetScene()
{
	App->scene->selectedGO = NULL;
	App->renderer->cameras.clear();
	App->renderer->lights.clear();
	App->renderer->meshes.clear();
	delete root;
	root = new GameObject("Root");
}

void ModuleScene::AddGameObject(System * system, rapidjson::Value& value)
{
	if (strcmp(system->GetString("parentUID", value), "0") != 0) {
		const char* parentUuid = system->GetString("parentUID", value);
		char uuidGameObjectParent[37];
		sprintf_s(uuidGameObjectParent, parentUuid);

		GameObject* parent = GetGameObjectByUUID(root, uuidGameObjectParent);

		GameObject* gameObject = new GameObject(parent);
		sprintf(gameObject->name, system->GetString("name", value));
		gameObject->Load(system, value);

		parent->childs.push_back(gameObject);
	}
	else 
	{
		root->Load(system, value);
	}
}

void ModuleScene::SaveScene()
{
	System* system = new System();
	
	// Save main camera
	/*if (primaryCamera != NULL)
	{
		system->AddName("primaryCamera");
		primaryCamera->Save(system);
	}*/

	// Save all gameobjects
	system->StartArray("GameObjects");
	SaveAllGameObjects(system, root);
	system->EndArray();

	system->WriteToDisk();

}

void ModuleScene::SaveAllGameObjects(System * config, GameObject * gameObject)
{
	gameObject->Save(config);

	for (list<GameObject*>::iterator it = gameObject->childs.begin(); it != gameObject->childs.end(); ++it)
	{
		SaveAllGameObjects(config, (*it));
	}
}

void ModuleScene::LoadScene()
{
	ResetScene();

	System* system = new System();
	rapidjson::Document document = system->LoadFromDisk();

	if (!document.HasParseError())
	{
		/*if(primaryCamera != NULL)
			primaryCamera->Load(system, document["primaryCamera"]);
		else
		{
			//primaryCamera = new GameObject("root");
		}*/

		rapidjson::Value gameObjects = document["GameObjects"].GetArray();
		for (rapidjson::Value::ValueIterator it = gameObjects.Begin(); it != gameObjects.End(); ++it) {
			AddGameObject(system, *it);
		}
	}

	delete system;
	system = NULL;
}
