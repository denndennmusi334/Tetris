#pragma once
#include "GameObject.h"
#include "BackGroundStar.h"

class BackGround :
    public GameObject
{
private:
	BackGroundStar* stars[Config::STAR_COUNT] = { nullptr };	//背景に表示する星の配列
	Animation* starIdleAnim = nullptr;							//背景に表示する星のアニメーション
	Animation* starDestroyAnim = nullptr;						//背景に表示する星の破壊アニメーション

	float spawnTimer = 0.0f;									//背景に表示する星の生成間隔を管理するタイマー
	const float SPAWN_INTERVAL = 0.5f;							//背景に表示する星の生成間隔(秒)

	BackGroundStar* mouseStar = nullptr;						//マウスカーソルに追従する星のオブジェクト
public:


	/// <summary>
	/// Animationを作成する関数.
	/// </summary>
	void CreateAnimation() override;


	/// <summary>
	/// ★が破壊されたらnullptrにする関数.
	/// </summary>
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


	/// <summary>
	/// ★をすべて破壊する関数.
	/// </summary>
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

