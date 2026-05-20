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

	float spawnTimer = 0.0f;       // 現在経過した時間（秒）
	const float SPAWN_INTERVAL = 0.5f; // ★ 星を出す間隔（例：0.5秒に1個ポコポコ出す）
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

