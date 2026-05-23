#pragma once
#include "BaseScene.h"
#include "MinoManager.h"
#include "BackGround.h"

class GameScene :
    public BaseScene
{
private:
	MinoManager minoMgr;
	BackGround* bg = nullptr;
public:
	GameScene(SceneController* sceneController);
	~GameScene();
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	void Kill() override;
};

