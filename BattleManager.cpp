#include "stdafx.h"
#include "BattleManager.h"
#include "TimeManager.h"
#include "InputManager.h"

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
		players[0].board = std::make_unique<MinoManager>();
		players[0].board->Initialize();

		break;
	case BattleMode::Double:
		players[0].board = std::make_unique<MinoManager>();
		players[1].board = std::make_unique<MinoManager>();
		players[0].board->SetBoardPosition(Vec2f{ 200.0f, 0 });
		players[1].board->SetBoardPosition(Vec2f{ Config::SCREEN_WIDTH / 2 + 200.0f, 0 });
		players[0].board->SetPlayerNumber(PlayerNumber::Player1);
		players[1].board->SetPlayerNumber(PlayerNumber::Player2);
		players[0].board->Initialize();
		players[1].board->Initialize();
		break;
	default:
		break;
	}

}

void BattleManager::Finalize()
{
}

void BattleManager::Update()
{
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

		while (!queue.empty())
		{
			queue.front().timer -=TimeManager::GetInstance().GetDeltaTime();

			if (queue.front().timer <= 0)
			{
				players[i].readyGarbage += queue.front().amount;
				queue.pop();
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
	preview->clear();
	auto& p = players[MyStd::ICast(player)];

	if (p.readyGarbage <= 0)
	{
		return;
	}

	p.board->ApplyGarbage(p.readyGarbage); p
}