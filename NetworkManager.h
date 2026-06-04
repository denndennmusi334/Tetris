#pragma once
#include "NetTetrisInput.h"
#include <mutex>

typedef std::string String;

struct RoomData
{
	String roomId;
	int matcheFormat;
	int availableSpots;
};

struct TetrisData
{
	int currentMinoX;
	int currentMinoY;
	int currentMinoType;
	int currentMinoRotateState;

	int map[Config::FIELD_HEIGHT][Config::FIELD_WIDTH];

	int holdMinoType;

	int nextMinoType;

	int power;

	bool isFixed;
};

struct BattleDataH
{
	TetrisData playerData[2];
};

struct BattleDataG
{
	NetworkInputState playerInputs;
};
/*    
	bool left;
    bool right;

    bool softDrop;
    bool hardDrop;

    bool rotateLeft;
    bool rotateRight;

    bool hold;
*/
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TetrisData, currentMinoX, currentMinoY, currentMinoType, currentMinoRotateState, map, holdMinoType, nextMinoType, power, isFixed)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BattleDataH, playerData)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NetworkInputState, left, right, softDrop, hardDrop, rotateLeft, rotateRight, hold)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BattleDataG, playerInputs)

class NetworkManager
{
private:
	NetworkManager() = default;
	~NetworkManager() = default;

	String roomId = "\0";
	String playerId;

	std::mutex mtx;
	std::mutex battleMutex;

	String nowRoomId = "\0";

	std::vector<RoomData> roomList;

	bool isHost = false;
	int roomMax = 0;

	ix::WebSocket ws;
	std::queue<nlohmann::json> recvQueue;

	BattleDataH battleDataH;
	std::queue<BattleDataG> battleDataG;

	bool roomReady = false;
	bool isStarted = false;
public:
	NetworkManager(const NetworkManager&) = delete;
	NetworkManager& operator=(const NetworkManager&) = delete;
	void Initialize();
	void Finalize();
	void Update();
	void Draw() {};

	void HandleMessage(const nlohmann::json& json);


	//void InitializeNetwork();

	void Logout();
	void Dismantle();

	void CreateRoom();
	bool JoinRoom(const String& roomId, const std::string& teamName, const std::string& playerName);
	void LoadRoomList();

	bool IsRoomReady() const { return roomReady; }
	bool IsHost() const { return isHost; }
	bool IsStarted() const { return isStarted; }

	void Send(const nlohmann::json& j)
	{
		ws.send(j.dump());
	}

	void SendBattleDataH(const BattleDataH& data)
	{
		std::lock_guard<std::mutex> lock(battleMutex);

		nlohmann::json j = data; 
		j["type"] = "battle_data_h";
		std::string str = j.dump();   
		ws.send(str);
	}

	void SendBattleDataG(const BattleDataG& data)
	{
		nlohmann::json j = data;
		j["type"] = "battle_data_g";
		std::string str = j.dump();
		ws.send(str);
	}

	void Connect();

	BattleDataH GetBattleDataH() {
		return battleDataH;
	}

	BattleDataG GetBattleDataG() {
		if (battleDataG.empty()) {
			static BattleDataG emptyData;
			return emptyData;
		}

		BattleDataG data = battleDataG.front();
		battleDataG.pop();
		return data;
	}

	bool HasBattleDataG() const {
		return !battleDataG.empty();
	}

	std::vector<RoomData>& GetRoomList() { return roomList; }

	std::string GetRoomId() { return roomId; }
	std::string GetPlayerId() { return playerId; }

	static NetworkManager& GetInstance() {
		static NetworkManager instance;
		return instance;
	}
};

