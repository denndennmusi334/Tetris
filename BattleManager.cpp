#include "stdafx.h"
#include "BattleManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "NetTetrisInput.h"
#include "GameObjectManager.h"

BattleManager::BattleManager()
{
}

BattleManager::~BattleManager()
{
}

void BattleManager::Initialize()
{
	switch (mode)
	{
	case BattleMode::Single:
		SInitialize();
		break;
	case BattleMode::Double:
		DInitialize();
		break;
	case BattleMode::Network:
		NInitialize();
		break;
	default:
		break;
	}

}

void BattleManager::SInitialize()
{
	KeyBinding p1 = {
		KEY_INPUT_A, // MoveLeft
		KEY_INPUT_D, // MoveRight
		KEY_INPUT_S, // SoftDrop
		KEY_INPUT_SPACE, // HardDrop
		KEY_INPUT_Q, // RotateLeft
		KEY_INPUT_E, // RotateRight
		KEY_INPUT_LSHIFT // Hold
	};


	players[0].board = std::make_unique<MinoManager>();
	players[0].input = std::make_unique<TetrisInput>(p1);
	players[0].board->SetTetrisInput(players[0].input.get());
	players[0].board->Initialize();
}

void BattleManager::DInitialize()
{
	KeyBinding p1 = {
	KEY_INPUT_A, // MoveLeft
	KEY_INPUT_D, // MoveRight
	KEY_INPUT_S, // SoftDrop
	KEY_INPUT_SPACE, // HardDrop
	KEY_INPUT_Q, // RotateLeft
	KEY_INPUT_E, // RotateRight
	KEY_INPUT_LSHIFT // Hold
	};

	KeyBinding p2 = {
		KEY_INPUT_NUMPAD4, // MoveLeft
		KEY_INPUT_NUMPAD6, // MoveRight
		KEY_INPUT_NUMPAD5, // SoftDrop
		KEY_INPUT_NUMPAD0, // HardDrop
		KEY_INPUT_NUMPAD7, // RotateLeft
		KEY_INPUT_NUMPAD9, // RotateRight
		KEY_INPUT_RETURN // Hold
	};

	players[0].board = std::make_unique<MinoManager>();
	players[1].board = std::make_unique<MinoManager>();
	players[0].board->SetBoardPosition(Vec2f{ 200.0f, 64.0f });
	players[1].board->SetBoardPosition(Vec2f{ Config::SCREEN_WIDTH / 2 + 200.0f, 64.0f });
	players[0].board->SetPlayerNumber(PlayerNumber::Player1);
	players[1].board->SetPlayerNumber(PlayerNumber::Player2);
	players[0].input = std::make_unique<TetrisInput>(p1);
	players[1].input = std::make_unique<TetrisInput>(p2);
	players[0].board->SetTetrisInput(players[0].input.get());
	players[1].board->SetTetrisInput(players[1].input.get());
	players[0].board->Initialize();
	players[1].board->Initialize();
}

void BattleManager::NInitialize()
{
	KeyBinding p1 = {
KEY_INPUT_A, // MoveLeft
KEY_INPUT_D, // MoveRight
KEY_INPUT_S, // SoftDrop
KEY_INPUT_SPACE, // HardDrop
KEY_INPUT_Q, // RotateLeft
KEY_INPUT_E, // RotateRight
KEY_INPUT_LSHIFT // Hold
	};


	players[0].board = std::make_unique<MinoManager>();
	players[1].board = std::make_unique<MinoManager>();
	players[0].board->SetBoardPosition(Vec2f{ 200.0f, 64.0f });
	players[1].board->SetBoardPosition(Vec2f{ Config::SCREEN_WIDTH / 2 + 200.0f, 64.0f });
	players[0].board->SetPlayerNumber(PlayerNumber::Player1);
	players[1].board->SetPlayerNumber(PlayerNumber::Player2);
	players[0].input = std::make_unique<TetrisInput>(p1);
	players[1].input = std::make_unique<NetTetrisInput>();
	players[0].board->SetTetrisInput(players[0].input.get());
	players[1].board->SetTetrisInput(players[1].input.get());
	players[0].board->Initialize();
	players[1].board->Initialize();
}

void BattleManager::Finalize()
{
}

void BattleManager::Update()
{
	for (int i = 0; i < 2; i++)
	{
		if (players[i].input)
		{
			players[i].input->Update();
		}
	}

	for (int i = 0; i < 2; i++)
	{
		if (players[i].board)
		{
			players[i].board->Update();
		}
	}

	for (int i = 0; i < 2; i++)
	{
		auto& queue =
			players[i].garbageQueue;

		if (!queue.empty())
		{
			queue.front().timer -=TimeManager::GetInstance().GetDeltaTime();

			if (queue.front().timer <= 0)
			{
				players[i].readyGarbage += queue.front().amount;
				queue.pop();
				RefreshPreview(MyStd::Cast<PlayerNumber>(i));
			}
		}
	}

	auto& input = InputManager::GetInstance();
	if (input.IsTrigger(KEY_INPUT_1))
	{
		SendGarbage(PlayerNumber::Player1, 1);
	}
	else if (input.IsTrigger(KEY_INPUT_2))
	{
		SendGarbage(PlayerNumber::Player1, 2);
	}
	else if (input.IsTrigger(KEY_INPUT_3))
	{ 
		SendGarbage(PlayerNumber::Player1, 3);
	}
	else if (input.IsTrigger(KEY_INPUT_4))
	{
		SendGarbage(PlayerNumber::Player1, 4);
	}
}

void BattleManager::Draw()
{

}

void BattleManager::RefreshPreview(PlayerNumber num)
{
	static const Vec2f PREVIEW_POS[2] = {
	Vec2f{ 200.0f, 0 },
	Vec2f{ Config::SCREEN_WIDTH / 2 + 200.0f, 0 }
	};

	for (auto& p : preview[MyStd::ICast(num)])
	{
		p->Destroy();
	}
	preview[MyStd::ICast(num)].clear();

	for (int i = 0; i < players[MyStd::ICast(num)].readyGarbage; i++)
	{
		Block* block = GameObjectManager::GetInstance().Create<Block>(BlockColor::WHITE, DrawType::Ghost, PREVIEW_POS[MyStd::ICast(num)]);
		preview[MyStd::ICast(num)].push_back(block);
	}

	int j = 0;
	for (auto& p : preview[MyStd::ICast(num)])
	{
		p->SetGridPosition({ 12,21 - j });
		j++;
	}
	
}

void BattleManager::SendGarbage(PlayerNumber from, int amount)
{
	PlayerNumber to = (from == PlayerNumber::Player1) ? PlayerNumber::Player2 : PlayerNumber::Player1;

	GarbageData data;
	data.amount = amount;
	data.timer = 0.5f;
	if (players[MyStd::Cast<int>(from)].readyGarbage > 0)
	{
		int remainingGarbage = data.amount - players[MyStd::Cast<int>(from)].readyGarbage;
		if (remainingGarbage < 0)
		{
			players[MyStd::Cast<int>(from)].readyGarbage -= data.amount;
			return;
		}
		else
		{
			data.amount = remainingGarbage;
			players[MyStd::Cast<int>(from)].readyGarbage = 0;
		}
	}
	if (data.amount <= 0)
	{
		return;
	}
	players[MyStd::Cast<int>(to)].garbageQueue.push(data);
}


void BattleManager::ApplyReadyGarbage(PlayerNumber player)
{
	for (auto& p : preview[MyStd::ICast(player)])
	{
		p->Destroy();
	}
	preview[MyStd::ICast(player)].clear();
	auto& p = players[MyStd::ICast(player)];

	if (p.readyGarbage <= 0)
	{
		return;
	}

	p.board->ApplyGarbage(p.readyGarbage);
	p.readyGarbage = 0;
}