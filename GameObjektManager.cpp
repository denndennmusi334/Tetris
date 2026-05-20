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

    if (!addBuffer.empty())
    {
        for (auto& newObj : addBuffer)
        {
            objects.push_back(std::move(newObj));
        }
        addBuffer.clear(); // ‘Ò‹@•”‰®‚ð‹ó‚É‚·‚é
    }
}


void GameObjectManager::Clear()
{
    objects.clear(); // unique_ptr ‚È‚Ì‚ÅˆêŠ‡‰ð•ú
}