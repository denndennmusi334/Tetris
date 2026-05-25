#pragma once
#include "MinoManager.h"

enum class BattleMode
{
	Single,
	Double
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

    std::vector<Block*> preview[2];
public:
    BattleManager(const BattleManager&) = delete;
    BattleManager& operator=(const BattleManager&) = delete;

    void SendGarbage(PlayerNumber from, int amount);
    void ApplyReadyGarbage(PlayerNumber player);

    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;

    void SetBattleMode(BattleMode _mode) { mode = _mode; }


    static BattleManager& GetInstance() {
        static BattleManager instance;
        return instance;
    }
};

