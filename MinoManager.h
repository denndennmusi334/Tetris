#pragma once
#include "MinoManager.h"
#include "GameMap.h"

enum class MinoAction
{
	MoveLeft,
	MoveRight,
	SoftDrop,
	HardDrop,
	RotateRight,
	RotateLeft,
	Hold
};

enum class PlayerNumber
{
	Player1,
	Player2
};

class MinoManager
{
private:
	PlayerNumber playerNumber = PlayerNumber::Player1;

	Tetromino* currentMino = nullptr;
	Tetromino* ghostMino = nullptr;
	Tetromino* nextMino = nullptr;
	std::unique_ptr<GameMap> gameMap = nullptr;

	double fallTimer = 0.0; //ミノが落ちるまでの時間を計測するタイマー.
	double fallInterval = 1.0; //ミノが落ちるまでの時間の間隔.初期値は1秒.

	std::vector<MinoType> bag;

	bool isEffectPlaying = false; 

	float lockTimer = 0.0f;
	float lockDelay = 0.5f;

	int score = 0;
	int totalLinesCleared = 0;

	bool lastActionIsRotate = false; // 最後の行動が「回転」だったか.
	int lastKickIndex = -1;          // 最後に成功したキックの番号 (0〜4).

	bool isGameOver = false;
	int level = 1;
	int linesForNextLevel = 10;
	const float SPEED_TABLE[16] = {
		1.000f, // Level 1.
		0.793f, // Level 2.
		0.618f, // Level 3.
		0.473f, // Level 4.
		0.355f, // Level 5.
		0.262f, // Level 6.
		0.190f, // Level 7.
		0.135f, // Level 8.
		0.094f, // Level 9.
		0.064f, // Level 10.
		0.043f, // Level 11.
		0.028f, // Level 12.
		0.018f, // Level 13.
		0.011f, // Level 14.
		0.007f, // Level 15.
		0.003f  // Level 16.
	};

	MinoType holdMinoType = MinoType::None;
	bool hasHeldInThisTurn = false; // ホールドは1ターンに1回までなので、そのフラグ.
	Tetromino* holdMinoVisual = nullptr;
	Vec2f boardOrigin = { Config::FIELD_X, Config::FIELD_Y };

#pragma region private関数
	bool IsMoveValid(const Vec2i& newPos) const; //ミノを移動させる前に、その移動が有効かどうかをチェックする関数.

	void MinoUpdate();
	void GhostUpdate();

	void HardDrop();

	void TryRotateRight();
	void TryRotateLeft();

	void LockMino(); //ミノを固定する関数.

	void RefillBag(); //テトリスの7種のミノをランダムに並べた袋をリフィルする関数.
	MinoType GetNextType(); //次のミノの種類をbagから取得する関数.

	void TestMino(const Vec2i& newPos) const; //ミノを移動させる前に、その移動が有効かどうかをチェックする関数.テスト用.

	void AddScore(int lineCount, bool isTspin, bool isMini);
	bool CheckTSpinCondition(int& outCornerCount, bool& outIsMini);

	void TryHold();

	void UpdateFallInterval();

	int CalculateAttack(int lineCount, bool isTSpin, bool isMini);

#pragma endregion
public:
	MinoManager();
	~MinoManager();
	void Initialize();
	void Finalize();
	void Update();
	void Draw(const Camera& camera);

	void SetBoardPosition(const Vec2f& pos) { boardOrigin = pos; }
	void SetPlayerNumber(PlayerNumber num) { playerNumber = num; }

	bool IsGameOver() const { return isGameOver; }

	int GetScore() const { return score; }

	void ApplyGarbage(int amount);

	bool IsEffectPlaying() { 
		bool is = isEffectPlaying;
		isEffectPlaying = false;
		return is;
	}

	Tetromino* GetCurrentMino() const { return currentMino; }
	Tetromino* GetNextMino() const { return nextMino; }

};

