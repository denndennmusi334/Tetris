#include "stdafx.h"
#include "BaseScene.h"
#include "SceneController.h"
#include "GameObjectManager.h"
BaseScene::BaseScene(SceneController* changer){
	sceneController = changer;
	GameObjectManager::GetInstance().SetDrawManager(&drawMgr);
}

