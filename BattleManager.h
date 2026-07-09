#pragma once
#include "MinoManager.h"
#include "TetrisInput.h"

/*
*   BattleManagerクラスは、テトリスのバトルモードを管理するクラスです。
*   対戦モードの種類（シングル、ダブル、ネットワーク）を管理し、各プレイヤーの状態や入力を処理します。
*   現在は、ネットワーク対戦のみをサポートしています。
*   シングルプレイやダブルプレイの実装は未完成です。
*   将来的にはAI対戦やCPUプレイヤーの追加も検討されています。
*/

enum class BattleMode
{
	Single,
	Double,
	Network
};

enum class ControlType
{
    Human,
    CPU,
    Remote
};

struct GarbageData
{
    int amount;

    float timer;
};

struct PlayerData
{
	std::unique_ptr<MinoManager> board;

	std::unique_ptr<TetrisInput> input;

    std::queue<GarbageData> garbageQueue;
    int readyGarbage = 0;

	bool dead = false;
} ;
class BattleManager : public Job
{
private:
    BattleManager();
	~BattleManager();
	PlayerData players[2];
	BattleMode mode = BattleMode::Single;
    ControlType controlType = ControlType::Human;

	std::unique_ptr<NetTetrisInput> netInput;

	bool isHost = false;

    std::vector<Block*> preview[2];

	int clientPower = 0;

    void RefreshPreview(PlayerNumber num);

	void SingleInitialize(); //シングルプレイの初期化.
	void DoubleInitialize(); //ダブルプレイの初期化.
	void NetworkInitialize(); //ネットワーク対戦の初期化.

public:
    BattleManager(const BattleManager&) = delete;
    BattleManager& operator=(const BattleManager&) = delete;

	void SendGarbage(PlayerNumber from, int amount);    //相手にゴミを送る関数. fromは送る側のプレイヤー番号, amountは送るゴミの量.
	void ApplyReadyGarbage(PlayerNumber player);        //溜まっているゴミを適用する関数. playerはゴミを適用するプレイヤー番号.

    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;

	void HostUpdate();      //ホスト側の更新処理. ゴミの送受信やプレイヤーの状態更新を行う.
	void ClientUpdate();    //クライアント側の更新処理. ホストからのデータを受信し、プレイヤーの状態を更新する.
	// 上二つはBattleModeがNetworkのときのみ使用される.

    void SetBattleMode(BattleMode _mode) { mode = _mode; }
	void SetIsHost(bool _isHost) { isHost = _isHost; }
	bool IsHost() const { return isHost; }


    static BattleManager& GetInstance() {
        static BattleManager instance;
        return instance;
    }
};

