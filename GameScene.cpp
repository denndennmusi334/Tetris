#include "stdafx.h"
#include "GameScene.h"
#include "GameObjectManager.h"
#include "ColliderManager.h"


GameScene::GameScene(SceneController* sceneController) : BaseScene(sceneController)
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	minoMgr.Initialize();
}

void GameScene::Finalize()
{
}

void GameScene::Update()
{
	minoMgr.Update();
	GameObjectManager::GetInstance().Update();
}

void GameScene::Draw()
{
	drawMgr.DrawAll(camera);
}

void GameScene::Kill()
{
	drawMgr.DestroyedDrawableCheck();

	ColliderManager::GetInstance().DestroyedColliderCheck();
	GameObjectManager::GetInstance().CheckDestroyedObjects();
}