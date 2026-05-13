#pragma once
#include "Job.h"
#include "BaseScene.h"
#include "SceneController.h"

class SceneManager :public SceneController, Job
{
private:
    BaseScene* Scene;
    BaseScene* PrevScene = nullptr;
    SceneName NextScene;

    bool requestQuit = false;

    void HandleSceneChange();
    BaseScene* CreateScene(SceneName name);
public:
    SceneManager()                     ;
    ~SceneManager()
    {
        if (Scene)
        {
            delete Scene;
        }

        if (PrevScene && PrevScene != Scene)
        {
            delete PrevScene;
        }

        Scene = nullptr;
        PrevScene = nullptr;
    }
    void Initialize()                   override ; //初期化
    void Finalize()                     override ; //終了処理
    void Update()                       override ; //更新
    void Draw()                         override ; //描画

    bool IsQuitRequested() const { return requestQuit; }

    void ChangeScene(SceneName NextScene)   override;// 引数 nextScene にシーンを変更する
    void BackScene()                        override;
    void RequestQuit()                      override;

};