#include "stdafx.h"
#include "GameObjectManager.h"

void GameObjectManager::Destroy(GameObject* obj)
{
	obj->Destroy();
}

void GameObjectManager::Update()
{
    for (auto& obj : objects)
        obj->Update();
}


void GameObjectManager::Clear()
{
    objects.clear(); // unique_ptr ‚È‚Ì‚ÅˆêŠ‡‰ð•ú
}