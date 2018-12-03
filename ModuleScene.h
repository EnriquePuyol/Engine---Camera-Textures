#ifndef __ModuleScene_h__
#define __ModuleScene_h__

#include "Module.h"
#include "GameObject.h"
#include <vector>

using namespace std;

class ModuleScene : public Module
{
public:
	ModuleScene();
	~ModuleScene();

	update_status PreUpdate() override;
	update_status Update() override;
	bool CleanUp() override;

	void CreateGameObject();
	void DeleteGameObject();

public:
	vector<GameObject*> gameobjects;
	GameObject* selectedGO = nullptr;

};

#endif // !__ModuleScene_h__