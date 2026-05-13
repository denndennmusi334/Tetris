#pragma once
#include "BaseScene.h"
#include "DrawManager.h"

class GameScene :
    public BaseScene
{
private:
public:
	GameScene(SceneController* sceneController);
	~GameScene();
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	void Kill() override;
};

