#pragma once
#include "MinoManager.h"
#include "GameMap.h"
class MinoManager
{
private:
	Tetromino* currentMino = nullptr;
	Tetromino* nextMino = nullptr;
	std::unique_ptr<GameMap> gameMap = nullptr;

	double fallTimer = 0.0; //ミノが落ちるまでの時間を計測するタイマー.
	double fallInterval = 1.0; //ミノが落ちるまでの時間の間隔.初期値は1秒.

	std::vector<MinoType> bag;

	float lockTimer = 0.0f;
	float lockDelay = 5.0f;
#pragma region private関数
	bool IsMoveValid(const Vec2i& newPos) const; //ミノを移動させる前に、その移動が有効かどうかをチェックする関数.

	void MinoUpdate();
	void HardDrop();

	void TryRotateRight();
	void TryRotateLeft();

	void LockMino(); //ミノを固定する関数.

	void RefillBag(); //テトリスの7種のミノをランダムに並べた袋をリフィルする関数.
	MinoType GetNextType(); //次のミノの種類をbagから取得する関数.

	void TestMino(const Vec2i& newPos) const; //ミノを移動させる前に、その移動が有効かどうかをチェックする関数.テスト用.
#pragma endregion
public:
	MinoManager();
	~MinoManager();
	void Initialize();
	void Finalize();
	void Update();
	void Draw(const Camera& camera);

	Tetromino* GetCurrentMino() const { return currentMino; }
	Tetromino* GetNextMino() const { return nextMino; }

};

