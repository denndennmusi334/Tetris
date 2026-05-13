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
}

void GameScene::Finalize()
{
}

void GameScene::Update()
{
}

void GameScene::Draw()
{
	drawMgr.DrawAll(camera);
}

void GameScene::Kill()
{
	ColliderManager::GetInstance().DestroyedColliderCheck();
	GameObjectManager::GetInstance().CheckDestroyedObjects();
}