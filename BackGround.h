#pragma once
#include "GameObject.h"
class BackGround :
    public GameObject
{
private:

public:
	BackGround();
	~BackGround();
	void Initialize() override;
	void Finalize() override {};
	void Update() override ;
	void Draw(const Camera& camera) override;
};

