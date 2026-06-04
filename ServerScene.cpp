#include "stdafx.h"
#include "ServerScene.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "BattleManager.h"

using namespace MyStd;

ServerScene::ServerScene(SceneController* changer) : BaseScene(changer)
{
}

ServerScene::~ServerScene()
{
}

void ServerScene::Initialize()
{
	BattleManager::GetInstance().SetBattleMode(BattleMode::Network);
	nameInput = GameObjectManager::GetInstance().Create<TextInputBox>(Vec2f{ Config::SCREEN_WIDTH / 32.0f, Config::SCREEN_HEIGHT / 4.0f * 3.0f }, 200.0f, 40.0f);
	roomListDrawPos = { Config::SCREEN_WIDTH / 8 * 5, Config::SCREEN_HEIGHT / 2 };
	roomListItemSize = { 800, 600 };
	buttonMgr.LoadCSV(TEXT("Resources/data/ButtonData.csv"));
	ChangeState(SSState::START);
}

void ServerScene::Finalize()
{
#if USE_BUTTON_EDIT
	buttonMgr.SaveCSV(TEXT("Resources/data/ButtonData.csv"));
#endif
	buttonMgr.Destroy();
}

void ServerScene::Update()
{
	buttonMgr.Update();
	NetworkManager::GetInstance().Update();
	switch (state)
	{
	case SSState::START:
		StartUpdate();
		break;
	case SSState::WAITING:
		WaitingUpdate();
		break;
	default:
		break;
	}

	GameObjectManager::GetInstance().Update();
}

void ServerScene::ChangeState(SSState newState)
{
	buttonMgr.SetAllActive(false);
	switch (newState)
	{
	case SSState::START:
		buttonMgr.SetActive((int)SSButtonID::SERVER_CREATE, true);
		buttonMgr.SetActive((int)SSButtonID::SERVER_JOIN, true);
		buttonMgr.SetActive((int)SSButtonID::SERVER_LOAD, true);
		nameInput->SetActive(true);
		break;
	case SSState::WAITING:
		buttonMgr.SetActive((int)SSButtonID::WAITING_EXIT, true);
		//buttonMgr.SetActive((int)SSButtonID::GO, true);
		nameInput->SetActive(false);
		break;
	default:
		break;
	}
	state = newState;
}

void ServerScene::StartUpdate()
{
	if (buttonMgr.IsRelease((int)SSButtonID::SERVER_CREATE))
	{
		NetworkManager::GetInstance().CreateRoom();
		roomList = NetworkManager::GetInstance().GetRoomList();
	}
	else if (buttonMgr.IsRelease((int)SSButtonID::SERVER_LOAD))
	{
		NetworkManager::GetInstance().LoadRoomList();
		roomList = NetworkManager::GetInstance().GetRoomList();
	}

	auto& iMgr = InputManager::GetInstance();
	if (selectRoomIndex > 0 && iMgr.IsRepeat(KEY_INPUT_UP, 10, 2)){
		selectRoomIndex--;
	}
	else if (selectRoomIndex < roomList.size() - 1 && iMgr.IsRepeat(KEY_INPUT_DOWN, 10, 2)){
		selectRoomIndex++;
	}

	if (buttonMgr.IsRelease((int)SSButtonID::SERVER_JOIN) && roomList.size() > 0)
	{
		String teamName = "team";
		teamName += std::to_string(MyStd::FRange(0.0f, 20000.0f));
		bool is = NetworkManager::GetInstance().JoinRoom(roomList[selectRoomIndex].roomId, teamName, nameInput->GetText());
		if (!is) { roomList = NetworkManager::GetInstance().GetRoomList(); }
		else { ChangeState(SSState::WAITING); }
	}
}

void ServerScene::WaitingUpdate()
{
	auto& iMgr = InputManager::GetInstance();
	if (buttonMgr.IsRelease(ICast(SSButtonID::WAITING_EXIT)))
	{
		NetworkManager::GetInstance().Logout();
		ChangeState(SSState::START);
	}
	if (buttonMgr.IsRelease(ICast(SSButtonID::GO))) {
		nlohmann::json j;
		j["type"] = "start";
		NetworkManager::GetInstance().Send(j);
		sceneController->ChangeScene(SceneName::GAME);
		BattleManager::GetInstance().SetIsHost(true);
	}

	if (!NetworkManager::GetInstance().IsHost()) {	//ホストでない場合は、部屋が準備できたことをサーバーに伝える.
		if (NetworkManager::GetInstance().IsStarted()) {
			BattleManager::GetInstance().SetIsHost(false);
			sceneController->ChangeScene(SceneName::GAME);
			return;
		}
		nlohmann::json j;
		j["type"] = "room_ready";
		NetworkManager::GetInstance().Send(j);
	}
	else {	//ホストは部屋が準備できたかどうかをサーバーから受け取る.
		if (NetworkManager::GetInstance().IsRoomReady()) {
			buttonMgr.SetActive((int)SSButtonID::GO, true);
		}
		else {
			buttonMgr.SetActive((int)SSButtonID::GO, false);
		}
	}
}


void ServerScene::Draw()
{
	drawMgr.DrawAll(camera);

	DrawBox(
		roomListDrawPos.x - roomListItemSize.x / 2,
		roomListDrawPos.y - roomListItemSize.y / 2,

		roomListDrawPos.x + roomListItemSize.x / 2,
		roomListDrawPos.y + roomListItemSize.y / 2,

		GetColor(200, 200, 200),
		TRUE
	);
	for (int i = 0; i < roomList.size(); i++)
	{
		unsigned int color = (i == selectRoomIndex) ? GetColor(155, 155, 0) : GetColor(0, 0, 0);
		color = (roomList[i].availableSpots == 0) ? GetColor(255, 100, 100) : color;

		std::wstring text =
			MyStd::Utf8ToWstring(roomList[i].roomId);

		text += L" 参加可能人数；";

		text += std::to_wstring(
			roomList[i].availableSpots);
		if (roomList[i].roomId == NetworkManager::GetInstance().GetRoomId()){
			text += L" Host";
		}

		DrawString(
			roomListDrawPos.x - roomListItemSize.x / 2 + 10,
			roomListDrawPos.y - roomListItemSize.y / 2 + 10 + i * 30,
			text.c_str(),
			color
		);
	}
}

void ServerScene::Kill()
{ 
	buttonMgr.Kill();
	GameObjectManager::GetInstance().CheckDestroyedObjects();
}