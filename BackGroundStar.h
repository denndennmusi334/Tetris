#pragma once
#include "GameObject.h"
class BackGroundStar :
    public GameObject
{
private:
	Vec2f vec;

	float speed = 50.0f;
	float radius = 3.0f;
public:
	BackGroundStar(Vec2f startPos, Vec2f _vec);
	~BackGroundStar();
	void Initialize() override;
	void Finalize() override {};
	void Update() override;
	void Draw(const Camera& camera) override;
};

