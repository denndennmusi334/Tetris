#include "stdafx.h"
#include "SceneManager.h"
#include "GameScene.h"

SceneManager::SceneManager() :NextScene(SceneName::INVALID)
{
	Scene = (BaseScene*) new GameScene(this);
	Scene->Initialize();
}

void SceneManager::Initialize()
{
	Scene->Initialize();
}

void SceneManager::Finalize()
{
    if (Scene == nullptr) return;

	Scene->Finalize();
    delete Scene;
    Scene = nullptr;
}

void SceneManager::Update()
{
	HandleSceneChange();

	if (Scene)
	{
		Scene->Update();
	}
}
void SceneManager::Draw()
{
	Scene->Draw();
}

void SceneManager::Kill()
{

    Scene->Kill();
    
}

void SceneManager::HandleSceneChange()
{
    if (NextScene == SceneName::INVALID) return;

    if (NextScene == SceneName::BACK)
    {
        std::swap(Scene, PrevScene);
        NextScene = SceneName::INVALID;
        return;
    }

    PrevScene = Scene;

    Scene = CreateScene(NextScene);

    if (Scene)
    {
        Scene->Initialize();
    }

    NextScene = SceneName::INVALID;
}

BaseScene* SceneManager::CreateScene(SceneName name)
{
    switch (name)
    {
    case SceneName::GAME: return new GameScene(this);
    default: return nullptr;
    }
}

void SceneManager::ChangeScene(SceneName _NextScene)
{
	if (_NextScene == SceneName::BACK) return;
    if(PrevScene != nullptr)PrevScene->Finalize();
	PrevScene = Scene;
	NextScene = _NextScene;
}

void SceneManager::BackScene()
{
	if (PrevScene == nullptr) return;

	NextScene = SceneName::BACK;
}

void SceneManager::RequestQuit()
{
    requestQuit = true;
}