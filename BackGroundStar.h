#pragma once
#include "GameObject.h"

typedef struct LineInfo
{
	Vec2f starPos;
	float alpha;
}LineInfo;

enum class StarState
{
	IDLE,
	DESTROY
};

class BackGroundStar :
    public GameObject
{
private:
	Vec2f vec;

	float speed = 50.0f;
	float radius = 3.0f;

	std::vector<LineInfo> starPoss;

	float dotAlpha = 255.0f;

	int LineAlphaCalculation(Vec2f _starPos);

	StarState state = StarState::IDLE;
	const Animation* idle, *destroy;
public:

	void Destroy();
	void SetAnimation(const Animation* _idle, const Animation* _destroy) {
		idle = _idle;
		destroy = _destroy;
		animator.ChangeAnimation(*idle, true);
	};

	void OnCollision(BaseCollider* other) override;
	BackGroundStar(Vec2f startPos, Vec2f _vec, float speed);
	~BackGroundStar();
	void Initialize() override;
	void Finalize() override {};
	void Update() override;
	void Draw(const Camera& camera) override;
};

