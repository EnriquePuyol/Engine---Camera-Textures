#ifndef __ModuleScene_h__
#define __ModuleScene_h__

#include "Module.h"
#include "GameObject.h"
#include "ComponentCamera.h"
#include "prettywriter.h"
#include "document.h"

class System;

class ModuleScene : public Module
{
public:
	ModuleScene();
	~ModuleScene();

	bool Init() override;
	update_status PreUpdate() override;
	update_status Update() override;
	bool CleanUp() override;

	void CreateGameObject();
	GameObject*	GetGameObjectByUUID(GameObject* gameObject, char uuidObjectName[37]);
	void DeleteGameObject();

	// FileSystem
	void ResetScene();
	void AddGameObject(System* system, rapidjson::Value& value);
	//   Save
	void SaveScene();
	void SaveAllGameObjects(System* config, GameObject* gameObject);
	//   Load
	void LoadScene();

public:
	GameObject* root;
	GameObject* selectedGO = nullptr;
	GameObject* cutcopyGO = nullptr;

	ComponentCamera* primaryCamera = NULL;

	//GameObject* lightGO = NULL;

};

#endif // !__ModuleScene_h__