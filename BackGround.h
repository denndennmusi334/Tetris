#pragma once
#include "GameObject.h"
#include "BackGroundStar.h"

class BackGround :
    public GameObject
{
private:
	BackGroundStar* stars[Config::STAR_COUNT] = { nullptr };
	Animation* starIdleAnim = nullptr;
	Animation* starDestroyAnim = nullptr;

	float spawnTimer = 0.0f;      
	const float SPAWN_INTERVAL = 0.5f;

	BackGroundStar* mouseStar = nullptr;
public:

	void CreateAnimation();

	void Kill() 
	{
		for (int i = 0; i < Config::STAR_COUNT; i++)
		{
			if (stars[i] == nullptr) continue;
			if (stars[i]->IsDestroyed())
			{
				stars[i] = nullptr;
			}
		}
		if (mouseStar->IsDestroyed()) {
			mouseStar = nullptr;
		}

	}

	void AllDestroy()
	{
		for (int i = 0; i < Config::STAR_COUNT; i++)
		{
			if (stars[i] != nullptr)
			{
				stars[i]->Destroy();
			}
		}
	}

	BackGround();
	~BackGround();
	void Initialize() override;
	void Finalize() override {};
	void Update() override ;
	void Draw(const Camera& camera) override;
};

