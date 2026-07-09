#pragma once
#include "MinoManager.h"
#include "GameMap.h"
#include "TetrisInput.h"
#include "NetworkManager.h"

#pragma region enum.

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
	Player2,
	Network
}; 

#pragma endregion

class MinoManager
{
private:
	PlayerNumber playerNumber = PlayerNumber::Player1;//自分がプレイヤー1かプレイヤー2か、ネットワーク対戦かを表す変数.

	TetrisInput* input = nullptr;				//入力を管理するクラスへのポインタ.外部で生成されたものをセットして使う.

	int ren = -1;								//連鎖数を表す変数.初期値は-1で、最初の1消しで0になるようにしている.

	bool backToBack = false;					//テトリスやTスピンを連続で決めたときのためのフラグ.最初はfalseで、テトリスやTスピンを決めるとtrueになる.
												//次にテトリスやTスピンを決めると、さらに強い攻撃になる.テトリスやTスピン以外の消しをすると、falseに戻る.	

	Tetromino* currentMino = nullptr;			//動かしているmino.
	Tetromino* ghostMino = nullptr;				//ゴーストミノ、現在のminoがどこまで落ちるかを表示するためのもの.
	Tetromino* nextMino = nullptr;				//次のminoを表示するためのもの.
	std::unique_ptr<GameMap> gameMap = nullptr;	//設置されたみのを保存しておくclass

	double fallTimer = 0.0; //ミノが落ちるまでの時間を計測するタイマー.
	double fallInterval = 1.0; //ミノが落ちるまでの時間の間隔.初期値は1秒.

	std::vector<MinoType> bag;					//テトリスの7種のミノをランダムに並べた袋.これから出てくるミノの種類は、
												//このbagから順番に取っていく. bagが空になったら、RefillBag関数を呼び出して新しいbagを作る.	

	bool isEffectPlaying = false;

	float lockTimer = 0.0f;						//ミノが着地してから固定されるまでの時間を計測するタイマー.
												//この時間内にミノを動かしたり回転させたりすると、固定されるのが遅れる.初期値は0で、ミノが着地してから増えていく.
	float lockDelay = 0.5f;						//ミノが着地してから固定されるまでの時間の間隔.初期値は0.5秒.

	int score = 0;
	int totalLinesCleared = 0;					//これまでに消したラインの合計.レベルアップの条件に使う.

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

	bool isFixed = false; // ネットワーク対戦で、ミノが固定されたかどうかを表すフラグ.

#pragma region private関数
	bool IsMoveValid(const Vec2i& newPos) const; //ミノを移動させる前に、その移動が有効かどうかをチェックする関数.
	/// <summary>
	/// MinoUpdate関数は、ミノの落下やプレイヤーの入力による移動・回転を処理する関数.この関数が呼び出されると、まずfallTimerを更新して、fallIntervalを超えているかどうかをチェックする.もし超えていれば、ミノを1マス下に移動させる.次に、プレイヤーの入力をチェックして、左右の移動や回転、ホールドなどのアクションを処理する.最後に、ミノが着地しているかどうかをチェックして、着地していればlockTimerを更新し、lockDelayを超えていればLockMino関数を呼び出してミノを固定する.
	/// </summary>
	void MinoUpdate();
	/// <summary>
	/// GhostUpdate関数は、ゴーストミノの位置を更新するための関数.この関数が呼び出されると、まずcurrentMinoの位置をghostMinoにコピーする.次に、ghostMinoを下に移動させ続けて、移動できなくなるまで落とす.これによって、ghostMinoはcurrentMinoがどこまで落ちるかを表示することができる.
	/// </summary>
	void GhostUpdate();

	/// <summary>
	/// HardDrop関数は、ミノを一気に落とすための関数.この関数が呼び出されると、ミノは現在の位置から下に移動し続け、移動できなくなるまで落ちる.ミノが落ちた後は、LockMino関数を呼び出してミノを固定する.
	/// </summary>
	void HardDrop();		
	//右回転を試みる関数.
	void TryRotateRight();
	//左回転を試みる関数.
	void TryRotateLeft();

	void LockMino(); //ミノを固定する関数.

	void RefillBag(); //テトリスの7種のミノをランダムに並べた袋をリフィルする関数.
	MinoType GetNextType(); //次のミノの種類をbagから取得する関数.

	void TestMino(const Vec2i& newPos, bool* isRotate) const; //ミノを移動させる前に、その移動が有効かどうかをチェックする関数.テスト用.

	void AddScore(int lineCount, bool isTspin, bool isMini);
	/// <summary>
	/// Tスピンをチェックする関数.
	/// </summary>
	/// <param name="outCornerCount"></param>
	/// <param name="outIsMini">TSpinMiniかを記録する変数</param>
	/// <returns></returns>
	bool CheckTSpinCondition(int& outCornerCount, bool& outIsMini);

	void TryHold();

	void UpdateFallInterval();

	int CalculateAttack(int lineCount, bool isTSpin, bool isMini);

	// TSpinの条件の一つである、ミノの4隅がいくつ埋まっているかを数える関数.この関数は、ミノの中心を基準にして、4隅の位置を計算し、その位置にブロックがあるかどうかをチェックする.埋まっている隅の数を返す.
	bool IsCornerFilled(const Vec2i& pos) const;

	void NetworkUpdate();

#pragma endregion
public:
	MinoManager();
	~MinoManager();
	void Initialize();
	void Finalize();
	
	void HostUpdate();
	void ClientUpdate();

	void Draw(const Camera& camera);

	void SetBoardPosition(const Vec2f& pos) { 
		boardOrigin = pos; 
	}
	void SetPlayerNumber(PlayerNumber num) { playerNumber = num; }

	bool IsGameOver() const { return isGameOver; }

	int GetScore() const { return score; }

	/// <summary>
	/// 攻撃を反映する関数
	/// </summary>
	/// <param name="amount">反映する攻撃の大きさ</param>
	void ApplyGarbage(int amount);

	void SetTetrisInput(TetrisInput* setInput) {
		input = setInput;
	}

	bool IsEffectPlaying() { 
		bool is = isEffectPlaying;
		isEffectPlaying = false;
		return is;
	}

	void ResetFixedFlag() { isFixed = false; }

	TetrisData GetTetrisDataH();

	Tetromino* GetCurrentMino() const { return currentMino; }
	Tetromino* GetNextMino() const { return nextMino; }

};

