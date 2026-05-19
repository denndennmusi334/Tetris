#pragma once
#include "BaseScene.h"
#include "MinoManager.h"

class GameScene :
    public BaseScene
{
private:
	MinoManager minoMgr;
public:
	GameScene(SceneController* sceneController);
	~GameScene();
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	void Kill() override;
};

