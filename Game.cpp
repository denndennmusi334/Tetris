#include "stdafx.h"
#include "Game.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "GameObjectManager.h"
#include "ColliderManager.h"
#include "ImageLoadManager.h"

void Game::Initialize()
{
    InitManager();

    sceneMgr = new SceneManager();
}

//‚±‚±‚Åinstance‚ð•K‚¸¢Š«‚µ‚Ä‚¨‚­•K—v‚È‚çinitialize‚à
void Game::InitManager() 
{
    TimeManager::GetInstance();
    InputManager::GetInstance();
    GameObjectManager::GetInstance();
    ColliderManager::GetInstance();
	ImageLoadManager::GetInstance().Initialize();
}

void Game::Finalize()
{
    if (sceneMgr)
    {
        sceneMgr->Finalize();
        delete sceneMgr;
        sceneMgr = nullptr;
    }
	ImageLoadManager::GetInstance().Finalize();
}

void Game::Draw()
{
    ClearDrawScreen();
    sceneMgr->Draw();
    ScreenFlip();
}

void Game::Update()
{
    sceneMgr->Update();
}

void Game::Kill()
{
	sceneMgr->Kill();
}

bool Game::Run()
{
    if (ProcessMessage() != 0) return false;

    if (sceneMgr && sceneMgr->IsQuitRequested()) return false;

    if (TimeManager::GetInstance().UpdateTiming())
    {
        InputManager::GetInstance().Update();
        Update();
        Draw();
        
		Kill();
    }

    return true;
}