#pragma once
#include "BaseScene.h"
#include "ButtonManager.h"
#include "NetworkManager.h"
#include "TextInputBox.h"

enum class SSButtonID
{
	SERVER_CREATE = 0,
	SERVER_JOIN,
	SERVER_LOAD,

	WAITING_EXIT,
	GO,

	COUNT
};

enum class SSState
{
	START,
	WAITING,
};

class ServerScene :
    public BaseScene
{
private:
	ButtonManager buttonMgr;
	SSState state = SSState::START;

	TextInputBox* nameInput = nullptr;

	int selectRoomIndex = 0;
	std::vector<RoomData> roomList;
	Vec2i roomListDrawPos = { Config::SCREEN_HEIGHT / 2, Config::SCREEN_WIDTH / 4 * 3 };
	Vec2i roomListItemSize = { 200, 20 };

	String HostRoomid = "\0"; //ホストが部屋を作成したときの部屋IDを保存する変数.これが"\0"でないときは、ホストは部屋を作成しているとみなす.
public:
	ServerScene(SceneController* changer);
	~ServerScene() override;
	void Initialize() override; //初期化.
	void Finalize() override; //終了処理.
	void Update() override; //更新.
	void Draw() override; //描画.
	void Kill() override;

	void ChangeState(SSState newState);
	void StartUpdate();
	void WaitingUpdate();
};

