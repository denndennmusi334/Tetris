#include "stdafx.h"
#include "GameScene.h"
#include "GameObjectManager.h"
#include "ColliderManager.h"
#include "InputManager.h"
#include "BattleManager.h"

GameScene::GameScene(SceneController* sceneController) : BaseScene(sceneController)
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	BattleManager::GetInstance().SetBattleMode(BattleMode::Double);
	BattleManager::GetInstance().Initialize();
	bg = GameObjectManager::GetInstance().Create<BackGround>();
}

void GameScene::Finalize()
{
}

void GameScene::Update()
{
	BattleManager::BattleManager::GetInstance().Update();
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
	DrawString(Config::FIELD_X + Config::FIELD_PIXEL_W, 200, ("SCORE: " + std::to_string(minoMgr.GetScore())).c_str(), GetColor(255, 255, 255));
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