#pragma once


enum class SceneName
{
	GAME,    //ゲームシーン.

    COUNT,

    BACK,    //一個前のシーンに戻る.

    INVALID = -1,
    BEGIN,
};

class SceneController
{
public:
    virtual ~SceneController() = 0;
    virtual void ChangeScene(SceneName nextScene) = 0;
    virtual void BackScene() = 0;

    virtual void RequestQuit() = 0;
};
