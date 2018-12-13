#ifndef __ModuleScene_h__
#define __ModuleScene_h__

#include "Module.h"
#include "GameObject.h"

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
	GameObject* root;
	GameObject* selectedGO = nullptr;
	GameObject* cutcopyGO = nullptr;

};

#endif // !__ModuleScene_h__