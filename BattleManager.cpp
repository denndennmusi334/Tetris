#include "stdafx.h"
#include "BattleManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "NetTetrisInput.h"
#include "NetworkManager.h"
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
		SingleInitialize();
		break;
	case BattleMode::Double:
		DoubleInitialize();
		break;
	case BattleMode::Network:
		NetworkInitialize();
		break;
	default:
		break;
	}

}

void BattleManager::SingleInitialize()
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

void BattleManager::DoubleInitialize()
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

void BattleManager::NetworkInitialize()
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

	if (NetworkManager::GetInstance().IsHost()) {
		players[0].board = std::make_unique<MinoManager>();
		players[1].board = std::make_unique<MinoManager>();
		players[0].board->SetBoardPosition(Vec2f{ 200.0f, 64.0f });
		players[1].board->SetBoardPosition(Vec2f{ Config::SCREEN_WIDTH / 2 + 200.0f, 64.0f });
		players[0].board->SetPlayerNumber(PlayerNumber::Player1);
		players[1].board->SetPlayerNumber(PlayerNumber::Player2);
		players[0].input = std::make_unique<TetrisInput>(p1);
		netInput = std::make_unique<NetTetrisInput>();
		players[0].board->SetTetrisInput(players[0].input.get());
		players[1].board->SetTetrisInput(dynamic_cast<TetrisInput*>(netInput.get()));
		players[0].board->Initialize();
		players[1].board->Initialize();
	}
	else {
		players[0].board = std::make_unique<MinoManager>();
		players[1].board = std::make_unique<MinoManager>();

		players[0].board->SetBoardPosition(Vec2f{ 200.0f, 64.0f });
		players[1].board->SetBoardPosition(Vec2f{ Config::SCREEN_WIDTH / 2 + 200.0f, 64.0f });

		players[0].board->SetPlayerNumber(PlayerNumber::Player2);
		players[1].board->SetPlayerNumber(PlayerNumber::Player1);

		players[0].input = std::make_unique<TetrisInput>(p1);
		players[0].board->SetTetrisInput(players[0].input.get());

		players[0].board->Initialize();
		players[1].board->Initialize();
	}


}

void BattleManager::Finalize()
{
}

void BattleManager::Update()
{
	if (mode == BattleMode::Network) {
		NetworkManager::GetInstance().Update();
	}
	if ((mode == BattleMode::Network && NetworkManager::GetInstance().IsHost()) || mode != BattleMode::Network)
	{
		HostUpdate();
	}
	else if (mode == BattleMode::Network && !NetworkManager::GetInstance().IsHost())
	{
		ClientUpdate();
	}

}


void BattleManager::HostUpdate()
{
	if (mode == BattleMode::Network) {
		auto& nMgr = NetworkManager::GetInstance();

		while (nMgr.HasBattleDataG())
		{
			BattleDataG data = nMgr.GetBattleDataG();
			netInput->SetState(data.playerInputs);
		}
		netInput->Update();
	}

	if (players[0].input)
	{
		players[0].input->Update();
	}

	for (int i = 0; i < 2; i++)
	{
		if (players[i].board)
		{
			players[i].board->HostUpdate();
		}
	}

	for (int i = 0; i < 2; i++)
	{
		auto& queue =
			players[i].garbageQueue;

		if (!queue.empty())
		{
			queue.front().timer -= TimeManager::GetInstance().GetDeltaTime();

			if (queue.front().timer <= 0)
			{
				players[i].readyGarbage += queue.front().amount;
				queue.pop();
				RefreshPreview(MyStd::Cast<PlayerNumber>(i));
			}
		}
	}

	if (mode == BattleMode::Network)
	{
		auto& nMgr = NetworkManager::GetInstance();

		BattleDataH data;
		for (int i = 0; i < 2; i++)
		{
			data.playerData[i] = players[i].board->GetTetrisDataH();
			data.playerData[i].power = players[i].readyGarbage;
		}
		nMgr.SendBattleDataH(data);

		for (int i = 0; i < 2; i++)
		{
			players[i].board->ResetFixedFlag();
		}

		netInput->ClearTriggers();
	}
}

void BattleManager::ClientUpdate()
{
	players[0].input->Update();

	BattleDataG data;

	// 既存のTetrisInputから状態をコピー
	data.playerInputs.left = players[0].input->IsPress(TetrisAction::MoveLeft);
	data.playerInputs.right = players[0].input->IsPress(TetrisAction::MoveRight);
	data.playerInputs.softDrop = players[0].input->IsPress(TetrisAction::SoftDrop);

	data.playerInputs.rotateLeft = players[0].input->IsTrigger(TetrisAction::RotateLeft);
	data.playerInputs.rotateRight = players[0].input->IsTrigger(TetrisAction::RotateRight);
	data.playerInputs.hold = players[0].input->IsTrigger(TetrisAction::Hold);
	data.playerInputs.hardDrop = players[0].input->IsTrigger(TetrisAction::HardDrop);

	NetworkManager::GetInstance().SendBattleDataG(data);

	if (players[0].board)
	{
		players[0].board->HostUpdate();
	}

	for (int i = 0; i < 2; i++)
	{
		if (players[i].board)
		{
			players[i].board->ClientUpdate();
		}
		if (players[i].readyGarbage != NetworkManager::GetInstance().GetBattleDataH().playerData[i].power)
		{
			players[i].readyGarbage = NetworkManager::GetInstance().GetBattleDataH().playerData[i].power;
			RefreshPreview(MyStd::Cast<PlayerNumber>(i));
		}
	}


}

void BattleManager::Draw()
{

}

void BattleManager::RefreshPreview(PlayerNumber num)
{

	float DrawX = 200.0f;
	static const Vec2f PREVIEW_POS[2] = {
	Vec2f{ DrawX, 0 },							//Player1のプレビューの描画位置.
	Vec2f{ Config::SCREEN_WIDTH / 2 + DrawX, 0 }//Player2のプレビューの描画位置.
	};

	//既存のプレビューを削除
	for (auto& p : preview[MyStd::ICast(num)])
	{
		p->Destroy();
	}
	preview[MyStd::ICast(num)].clear();

	//新しいプレビューを作成
	for (int i = 0; i < players[MyStd::ICast(num)].readyGarbage; i++)
	{
		Block* block = GameObjectManager::GetInstance().Create<Block>(BlockColor::WHITE, DrawType::Ghost, PREVIEW_POS[MyStd::ICast(num)]);
		preview[MyStd::ICast(num)].push_back(block);
	}

	//プレビューの位置を調整
	int j = 0;
	const int BLOCK_X = 12; //preview内のブロックのX座標のオフセット
	const int BLOCK_Y = 21; //preview内のブロックのY座標のオフセット.ブロックが1つ増えるごとにY座標が1減るようにするため、初期値は21にしている.
	for (auto& p : preview[MyStd::ICast(num)])
	{
		p->SetGridPosition({ BLOCK_X,BLOCK_Y - j });
		j++;
	}
	
}

void BattleManager::SendGarbage(PlayerNumber from, int amount)
{
	//自分から見た相手のPlayerNumberを求める.例えば、fromがPlayer1なら、toはPlayer2になる.
	PlayerNumber to = (from == PlayerNumber::Player1) ? PlayerNumber::Player2 : PlayerNumber::Player1;

	const float AMOUNT_MINO_TIME = 0.5f; //ゴミが相手に届くまでの時間.適宜調整してください.

	GarbageData data;
	data.amount = amount;
	data.timer = AMOUNT_MINO_TIME;
	if (players[MyStd::Cast<int>(from)].readyGarbage > 0)
	{
		//相手に送るゴミの量から、自分がすでに溜めているゴミの量を引く.例えば.
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
	//もし送るゴミの量が0以下になってしまったら、何もしないで返す.
	if (data.amount <= 0)
	{
		return;
	}
	//相手のゴミキューに、送るゴミの量とタイマーをセットしたGarbageDataを追加する.
	players[MyStd::Cast<int>(to)].garbageQueue.push(data);
}


void BattleManager::ApplyReadyGarbage(PlayerNumber player)
{
	//既存のプレビューを削除
	for (auto& p : preview[MyStd::ICast(player)])
	{
		p->Destroy();
	}
	//プレビューのベクターをクリア
	preview[MyStd::ICast(player)].clear();
	auto& p = players[MyStd::ICast(player)];

	//溜まっているゴミの量が0以下なら、何もしないで返す.
	if (p.readyGarbage <= 0)
	{
		return;
	}

	p.board->ApplyGarbage(p.readyGarbage);
	p.readyGarbage = 0;
}