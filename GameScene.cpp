#include "stdafx.h"
#include "GameScene.h"
#include "GameObjectManager.h"
#include "ColliderManager.h"
#include "InputManager.h"

GameScene::GameScene(SceneController* sceneController) : BaseScene(sceneController)
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	minoMgr.Initialize();
	bg = GameObjectManager::GetInstance().Create<BackGround>();
}

void GameScene::Finalize()
{
}

void GameScene::Update()
{
	minoMgr.Update();
	GameObjectManager::GetInstance().Update();

	if (minoMgr.IsEffectPlaying())
	{
		bg->AllDestroy();
	}

	ColliderManager::GetInstance().CheckAllCollisions();
}

void GameScene::Draw()
{
	drawMgr.DrawAll(camera);
	DrawString(Config::FIELD_X + Config::FIELD_PIXEL_W, 200, (L"SCORE: " + std::to_wstring(minoMgr.GetScore())).c_str(), GetColor(255, 255, 255));
#if DEBUG
	ColliderManager::GetInstance().DebugDraw();
#endif
}

void GameScene::Kill()
{
	drawMgr.DestroyedDrawableCheck();
	bg->Kill();

	ColliderManager::GetInstance().DestroyedColliderCheck();
	GameObjectManager::GetInstance().CheckDestroyedObjects();
}