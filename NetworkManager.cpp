#include "stdafx.h"
#include "NetworkManager.h"

using json = nlohmann::json;

void NetworkManager::Initialize()
{
	isHost = false;
	roomReady = false;
	isStarted = false;
}

void NetworkManager::Finalize()
{
	Dismantle();
	if (nowRoomId != "\0" && !isHost){
		Logout();
	}
}
#pragma region http退出

void NetworkManager::Logout()
{
	httplib::Client cli("http://10.22.6.170:8080");

	httplib::Params params;

	params.emplace("room_id", nowRoomId);
	params.emplace("player_id", playerId);

	auto res = cli.Post(
		"/logout",
		params
	);
	nowRoomId = "\0";
	ws.stop();
}

void NetworkManager::Dismantle()
{
	if (roomId == "\0") return;
	httplib::Client cli("http://10.22.6.170:8080");

	httplib::Params params;

	params.emplace("room_id", roomId);
	params.emplace("player_id", playerId);

	auto res = cli.Post(
		"/dismantle",
		params
	);
	ws.stop();
}
#pragma endregion
void NetworkManager::Update()
{
	roomReady = false;
	while (true)
	{
		json j;

		{
			std::lock_guard<std::mutex> lock(mtx);

			if (recvQueue.empty())
				break;

			j = recvQueue.front();
			recvQueue.pop();
		}

		HandleMessage(j);
	}
}

void NetworkManager::HandleMessage(const nlohmann::json& json)
{
	std::string type =
		json.value("type", "");

	if (type == "room_ready")
	{
		roomReady = true;
	}
	else if (type == "start")
	{
		isStarted = true;
	}

	if (type == "battle_data_h")
	{
		BattleDataH data = { 0 };



		if (json.contains("playerData") && json["playerData"].is_array())
		{
			auto& pList = json["playerData"];

			int playerCount = std::min((int)pList.size(), 2);

			for (int i = 0; i < playerCount; i++)
			{
				for (int y = 0; y < Config::FIELD_HEIGHT; y++)
				{
					for (int x = 0; x < Config::FIELD_WIDTH; x++)
					{
						data.playerData[i].map[y][x] = -1;
					}
				}

				auto& pData = pList[i];

				//printfDx(L"map exists=%d\n",
				//	pData.contains("map"));

				data.playerData[i].currentMinoRotateState = pData.value("currentMinoRotateState", 0);
				data.playerData[i].currentMinoType = pData.value("currentMinoType", 0);
				data.playerData[i].currentMinoX = pData.value("currentMinoX", 0);
				data.playerData[i].currentMinoY = pData.value("currentMinoY", 0);

				data.playerData[i].holdMinoType = pData.value("holdMinoType", -1);
				data.playerData[i].nextMinoType = pData.value("nextMinoType", 0);
				data.playerData[i].power = pData.value("power", 0);
				if (pData.contains("map") && pData["map"].is_array())
				{
					auto& jsonMap = pData["map"];
					int height = std::min((int)jsonMap.size(), Config::FIELD_HEIGHT);

					for (int y = 0; y < height; y++)
					{
						if (jsonMap[y].is_array())
						{
							int width = std::min((int)jsonMap[y].size(), Config::FIELD_WIDTH);
							for (int x = 0; x < width; x++)
							{
								if (jsonMap[y][x].is_number_integer())
								{
									data.playerData[i].map[y][x]
										= jsonMap[y][x].get<int>();
								}
								else
								{
									data.playerData[i].map[y][x] = -1;
								}
							}
						}
					}
				}

				data.playerData[i].isFixed = pData.value("isFixed", false);
			}
		}

		std::lock_guard<std::mutex> lock(battleMutex);

		battleDataH = data;
	}
	else if (type == "battle_data_g")
	{
		BattleDataG data;
		data.playerInputs = json["playerInputs"].get<NetworkInputState>();
		battleDataG.push(data);
	}
}

#pragma region http参加

void NetworkManager::CreateRoom()
{
	Dismantle();
	httplib::Client cli("http://10.22.6.170:8080");

	httplib::Params params;

	params.emplace("match_format_id", "1");
	auto res = cli.Post(
		"/init",
		params
	);

	std::wstring str = MyStd::Utf8ToWstring(res->body);

#if _DEBUG
	OutputDebugStringW(str.c_str());
	OutputDebugStringW(L"\n");
#endif

	if (res)
	{
		auto json = nlohmann::json::parse(res->body);

		this->playerId = json["player_id"];
		roomId = json["room_id"];

		RoomData data;
		data.roomId = roomId;
		data.matcheFormat = 1;
		data.availableSpots = 2;
		LoadRoomList();
	}
	else
	{
		//printfDx(L"connection failed\n");
		return;
	}
}

bool NetworkManager::JoinRoom(const String& roomId, const std::string& teamName, const std::string& playerName)
{
	if (playerName == "\0") {
		return false;
	}
	LoadRoomList();

	for (auto& room : roomList)
	{
		if (room.roomId == roomId && room.availableSpots <= 0) 
		{
			return false;	
		}
	}
    httplib::Client cli("http://10.22.6.170:8080");

    httplib::Params params;

	params.emplace("room_id", roomId);
	params.emplace("player_id", playerId);
	params.emplace("team_name", teamName);
	params.emplace("user_name", playerName);
	auto res = cli.Post(
		"/join",
		params
	);

	std::wstring str = MyStd::Utf8ToWstring(res->body);

#if _DEBUG
	OutputDebugStringW(str.c_str());
	OutputDebugStringW(L"\n");
#endif

	if (res)
	{
		auto json = nlohmann::json::parse(res->body);
		
		nowRoomId = roomId;
		isHost = json["is_host"];
		this->playerId = json["player_id"];
	}
	else
	{
		//printfDx(L"connection failed\n");
		return false;
	}

	Connect();
	return true;
}

void NetworkManager::LoadRoomList()
{
	roomList.clear();
	httplib::Client cli("http://10.22.6.170:8080");

	auto res = cli.Get("/roomlist");

	if (res)
	{
		auto json = nlohmann::json::parse(res->body);

		auto rooms = json["rooms"];
		for (auto& room : rooms)
		{
			RoomData data;
			data.roomId = room["room_id"];
			data.matcheFormat = room["match_format"];
			data.availableSpots = room["available_spots"];
			roomList.push_back(data);
		}

	}
	else
	{
		//printfDx(L"connection failed\n");
		return;
	}
}

#pragma endregion

// WebSocket接続とメッセージ受信の設定
void NetworkManager::Connect()
{
	std::string url =
		"ws://10.22.6.170:8080/ws?room_id=" +
		nowRoomId +
		"&player_id=" +
		playerId;

	ws.setUrl(url);


	ws.setOnMessageCallback(
		[this](const ix::WebSocketMessagePtr& msg)
		{
			switch (msg->type)
			{
			case ix::WebSocketMessageType::Open:
#if _DEBUG
				printfDx(L"Connected!\n");
#endif
				ws.send("hello");
				break;

			case ix::WebSocketMessageType::Message:
			{
				try
				{
					auto j = nlohmann::json::parse(msg->str);

					{
						std::lock_guard<std::mutex> lock(mtx);
						recvQueue.push(j);
					} 
				}
				catch (...)
				{
#if _DEBUG
					printfDx(L"JSON Parse Error!\n");
#endif
				}
				break;
			}

			case ix::WebSocketMessageType::Close:
#if _DEBUG
				printfDx(L"Closed\n");
#endif
				break;

			case ix::WebSocketMessageType::Error:
#if _DEBUG
				printfDx(L"Error : %s\n", MyStd::Utf8ToWstring(msg->errorInfo.reason).c_str());
#endif
				break;
			}
		});

	ws.start();
}