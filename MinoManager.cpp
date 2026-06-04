#include "stdafx.h"
#include "MinoManager.h"
#include "GameObjectManager.h"
#include "TimeManager.h"
#include "BattleManager.h"
#include "NetworkManager.h"

using namespace MyStd;



MinoManager::MinoManager()
{
}

MinoManager::~MinoManager()
{
}

void MinoManager::Initialize()
{
	RefillBag();
	currentMino = GameObjectManager::GetInstance().Create<Tetromino>(GetNextType(), boardOrigin);
	currentMino->SetGridPosition(Vec2i{ 5, 0 }); //フィールドの中央上部にミノを表示するための位置.

	ghostMino = GameObjectManager::GetInstance().Create<Tetromino>(currentMino->GetType(), boardOrigin, DrawType::Ghost);

	nextMino = GameObjectManager::GetInstance().Create<Tetromino>(GetNextType(), boardOrigin);
	nextMino->SetGridPosition(Vec2i{ Config::FIELD_WIDTH + 2, 2 }); //フィールドの右側に次のミノを表示するための位置.

	gameMap = std::make_unique<GameMap>();
	gameMap->SetBoardPos(boardOrigin);
	level = 1;
	linesForNextLevel = 10;
	UpdateFallInterval();
}

void MinoManager::Finalize()
{
}

void MinoManager::HostUpdate()
{
	if (isGameOver)return;
	MinoUpdate();
	GhostUpdate();
}

void MinoManager::ClientUpdate()
{
	if (isGameOver)return;
	NetworkUpdate();
	GhostUpdate();
}

void MinoManager::NetworkUpdate()
{
	if (isGameOver) return;

	int idx = ICast(playerNumber);
	TetrisData data = NetworkManager::GetInstance().GetBattleDataH().playerData[idx];
	if (playerNumber == PlayerNumber::Player1)
	{
		currentMino->SetGridPosition(Vec2i(data.currentMinoX, data.currentMinoY));
		currentMino->SetType(Cast<MinoType>(data.currentMinoType));
		currentMino->SetRotateState(Cast<RotateState>(data.currentMinoRotateState));
		gameMap->SetMapData(data.map);
	}

	if (playerNumber == PlayerNumber::Player2)
	{
		bool isFirstFrame = (nextMino->GetType() != Cast<MinoType>(data.nextMinoType));

		if (data.isFixed || isFirstFrame)
		{
			currentMino->SetType(Cast<MinoType>(data.currentMinoType));
			currentMino->SetGridPosition(Vec2i(data.currentMinoX, data.currentMinoY));
			currentMino->SetRotateState(Cast<RotateState>(data.currentMinoRotateState));
			gameMap->SetMapData(data.map);
		}

	}


	if (nextMino->GetType() != Cast<MinoType>(data.nextMinoType)) {
		nextMino->SetType(Cast<MinoType>(data.nextMinoType));
	}

	MinoType newHoldMinoType = Cast<MinoType>(data.holdMinoType);
	if (holdMinoType != newHoldMinoType)
	{
		holdMinoType = newHoldMinoType;
		if (!holdMinoVisual)
		{
			holdMinoVisual = GameObjectManager::GetInstance().Create<Tetromino>(holdMinoType, boardOrigin);
			holdMinoVisual->SetGridPosition(Vec2i{ -4, 2 });
		}
		else
		{
			holdMinoVisual->SetType(holdMinoType);
		}
	}
}

void MinoManager::Draw(const Camera& camera)
{
}

void MinoManager::MinoUpdate()
{
	if (isGameOver)return;

	auto Gpos = currentMino->GetGridPosition();
	if (input->IsTrigger(TetrisAction::RotateRight))
	{
		lockTimer = 0.0f;
		TryRotateRight();
		lastActionIsRotate = true;
	}
	else if (input->IsTrigger(TetrisAction::RotateLeft))
	{
		lockTimer = 0.0f;
		TryRotateLeft();
		lastActionIsRotate = true;
	}
	else if (input->IsRepeat(TetrisAction::MoveLeft, 8, 2))
	{
		lockTimer = 0.0f;
		TestMino(Vec2i(-1, 0), &lastActionIsRotate);
	}
	else if (input->IsRepeat(TetrisAction::MoveRight, 8, 2))
	{
		lockTimer = 0.0f;
		TestMino(Vec2i(1, 0), &lastActionIsRotate);
	}
	else if (input->IsRepeat(TetrisAction::SoftDrop, 8, 2))
	{
		lockTimer = 0.0f;
		TestMino(Vec2i(0, 1), &lastActionIsRotate);
	}
	else if (input->IsTrigger(TetrisAction::HardDrop))
	{
		HardDrop();
	}
	else if (input->IsTrigger(TetrisAction::Hold))
	{
		TryHold();
	}

	if (fallTimer >= fallInterval)
	{
		TestMino(Vec2i(0, 1), &lastActionIsRotate);
		fallTimer = 0.0;
	}
	else
	{
		fallTimer += TimeManager::GetInstance().GetDeltaTime();
	}

	if (!IsMoveValid(currentMino->GetGridPosition() + Vec2i(0, 1)))
	{
		lockTimer += TimeManager::GetInstance().GetDeltaTime();

		if (lockTimer >= lockDelay)
		{
			LockMino();
			lockTimer = 0.0f;
		}
	}
	else
	{
		lockTimer = 0.0f;
	}

}

void MinoManager::GhostUpdate()
{
	if (isGameOver)return;
	ghostMino->CopyTransFrom(*currentMino);
	Vec2i pos = ghostMino->GetGridPosition();
	while (IsMoveValid(pos + Vec2i(0, 1)))
	{
		pos.y += 1;
	}
	ghostMino->SetGridPosition(pos);
}

void MinoManager::TryHold()
{
	if (hasHeldInThisTurn) return;

	MinoType currentType = currentMino->GetType();

	if (holdMinoType == MinoType::None)
	{
		holdMinoType = currentType;
		currentMino->SetRotateState(RotateState::UP);
		holdMinoVisual = currentMino;
		currentMino = nextMino;
		currentMino->SetGridPosition(Vec2i{ 5, 0 });

		nextMino = GameObjectManager::GetInstance().Create<Tetromino>(GetNextType(), boardOrigin);
		nextMino->SetGridPosition(Vec2i{ Config::FIELD_WIDTH + 2, 2 });
	}
	else
	{
		MinoType prevHoldType = holdMinoType;
		holdMinoType = currentType;

		currentMino->SetRotateState(RotateState::UP);
		Tetromino* newHoldMino = currentMino;
		currentMino = holdMinoVisual;
		holdMinoVisual = newHoldMino;

		currentMino->SetGridPosition(Vec2i{ 5, 0 });
	}

	holdMinoVisual->SetGridPosition(Vec2i{ -4, 2 });

	hasHeldInThisTurn = true;

	fallTimer = 0.0f;
	lockTimer = 0.0f;
}

void MinoManager::TestMino(const Vec2i& newPos, bool* isRotate) const
{
	Vec2i testPos = currentMino->GetGridPosition() + newPos;
	if (IsMoveValid(testPos))
	{ 
		currentMino->SetGridPosition(testPos);
		*isRotate = false;
	}
}
void MinoManager::RefillBag()
{
	bag =
	{
		MinoType::I,
		MinoType::J,
		MinoType::L,
		MinoType::O,
		MinoType::S,
		MinoType::T,
		MinoType::Z
	};

	std::shuffle(
		bag.begin(),
		bag.end(),
		GetEngine());
}

MinoType MinoManager::GetNextType()
{
	if (bag.empty())
	{
		RefillBag();
	}

	MinoType type = bag.back();
	bag.pop_back();

	return type;
}
void MinoManager::HardDrop()
{
	Vec2i pos =
		currentMino->GetGridPosition();

	while (true)
	{
		Vec2i next = pos;
		next.y += 1;

		if (!IsMoveValid(next))
		{
			break;
		}

		pos = next;
	}

	currentMino->SetGridPosition(pos);

	// 固定
	LockMino();
}

void MinoManager::LockMino()
{
	gameMap->SetBlock(currentMino);

	isFixed = true;

	int cornerCount = 0;
	bool isMini = false;
	bool isTspin = CheckTSpinCondition(cornerCount, isMini);

	int clearedLines = gameMap->BreakBlockCheck();

	AddScore(clearedLines, isTspin, isMini);

	int attack =
		CalculateAttack(clearedLines, isTspin, isMini);
	
	BattleManager::GetInstance().SendGarbage(playerNumber, attack);
	BattleManager::GetInstance().ApplyReadyGarbage(playerNumber);

	nextMino->SetGridPosition(Vec2i{ 5, 0 });
	currentMino = nextMino;
	nextMino = GameObjectManager::GetInstance().Create<Tetromino>(GetNextType(), boardOrigin);
	nextMino->SetGridPosition(Vec2i{ Config::FIELD_WIDTH + 2, 2 });

	if (!IsMoveValid(currentMino->GetGridPosition())) { isGameOver = true; }

	lastActionIsRotate = false;
	hasHeldInThisTurn = false;
	lastKickIndex = -1;
}

#pragma region 回転部分SRS

static constexpr Vec2i JLSTZ_KICK[4][4][5] =
{
	// UP (0)
	{
		{}, // UP->UP

		// UP -> RIGHT (0 -> 1)
		{
			{0,0},
			{-1,0},
			{-1,-1}, // 元 1 → 修正 -1 (上へキック)
			{0,2},   // 元 -2 → 修正 2 (下へキック)
			{-1,2},  // 元 -2 → 修正 2
		},

		{},

		// UP -> LEFT (0 -> 3)
		{
			{0,0},
			{1,0},
			{1,-1}, // 元 1 → 修正 -1
			{0,2},  // 元 -2 → 修正 2
			{1,2},  // 元 -2 → 修正 2
		}
	},

	// RIGHT (1)
	{
		// RIGHT -> UP (1 -> 0)
		{
			{0,0},
			{1,0},
			{1,1},   // 元 -1 → 修正 1
			{0,-2},  // 元 2 → 修正 -2
			{1,-2},  // 元 2 → 修正 -2
		},

		{},

		// RIGHT -> DOWN (1 -> 2)
		{
			{0,0},
			{1,0},
			{1,1},   // 元 -1 → 修正 1
			{0,-2},  // 元 2 → 修正 -2
			{1,-2},  // 元 2 → 修正 -2
		},

		{},
	},

	// DOWN (2)
	{
		{},

		// DOWN -> RIGHT (2 -> 1)
		{
			{0,0},
			{-1,0},
			{-1,-1}, // 元 1 → 修正 -1
			{0,2},   // 元 -2 → 修正 2
			{-1,2},  // 元 -2 → 修正 2
		},

		{},

		// DOWN -> LEFT (2 -> 3)
		{
			{0,0},
			{1,0},
			{1,-1},  // 元 1 → 修正 -1
			{0,2},   // 元 -2 → 修正 2
			{1,2},   // 元 -2 → 修正 2
		}
	},

	// LEFT (3)
	{
		// LEFT -> UP (3 -> 0)
		{
			{0,0},
			{-1,0},
			{-1,1},  // 元 -1 → 修正 1
			{0,-2},  // 元 2 → 修正 -2
			{-1,-2}, // 元 2 → 修正 -2
		},

		{},

		// LEFT -> DOWN (3 -> 2)
		{
			{0,0},
			{-1,0},
			{-1,1},  // 元 -1 → 修正 1
			{0,-2},  // 元 2 → 修正 -2
			{-1,-2}, // 元 2 → 修正 -2
		},

		{},
	},
};

static constexpr Vec2i I_KICK[4][4][5] =
{
	// UP
	{
		{},

		// UP -> RIGHT
		{
			{0,0},
			{-2,0},
			{1,0},
			{-2,1},
			{1,-2},
		},

		{},

		// UP -> LEFT
		{
			{0,0},
			{-1,0},
			{2,0},
			{-1,-2},
			{2,1},
		}
	},

	// RIGHT
	{
		// RIGHT -> UP
		{
			{0,0},
			{2,0},
			{-1,0},
			{2,-1},
			{-1,2},
		},

		{},

		// RIGHT -> DOWN
		{
			{0,0},
			{-1,0},
			{2,0},
			{-1,-2},
			{2,1},
		},

		{},
	},

	// DOWN
	{
		{},

		// DOWN -> RIGHT
		{
			{0,0},
			{1,0},
			{-2,0},
			{1,2},
			{-2,-1},
		},

		{},

		// DOWN -> LEFT
		{
			{0,0},
			{2,0},
			{-1,0},
			{2,-1},
			{-1,2},
		}
	},

	// LEFT
	{
		// LEFT -> UP
		{
			{0,0},
			{1,0},
			{-2,0},
			{1,2},
			{-2,-1},
		},

		{},

		// LEFT -> DOWN
		{
			{0,0},
			{-2,0},
			{1,0},
			{-2,1},
			{1,-2},
		},

		{},
	},
};

void MinoManager::TryRotateRight()
{
	RotateState from =
		currentMino->GetRotateState();

	RotateState to =
		(RotateState)(((int)from + 1) % 4);

	Vec2i originalPos =
		currentMino->GetGridPosition();

	currentMino->RotateRightRaw();

	const Vec2i* kick;

	if (currentMino->GetType() ==
		MinoType::I)
	{
		kick = I_KICK[(int)from][(int)to];
	}
	else
	{
		kick = JLSTZ_KICK[(int)from][(int)to];
	}

	for (int i = 0; i < 5; i++)
	{
		Vec2i testPos =
			originalPos + kick[i];

		if (IsMoveValid(testPos))
		{
			currentMino->SetGridPosition(testPos);
			lastKickIndex = i; // 最後に成功したキックの番号を保存
			return;
		}
	}

	// 全失敗
	currentMino->RotateLeftRaw();
	currentMino->SetGridPosition(originalPos);
}

void MinoManager::TryRotateLeft()
{
	RotateState from =
		currentMino->GetRotateState();

	RotateState to =
		(RotateState)(((int)from + 3) % 4);

	Vec2i originalPos =
		currentMino->GetGridPosition();

	currentMino->RotateLeftRaw();

	const Vec2i* kick;

	if (currentMino->GetType() ==
		MinoType::I)
	{
		kick = I_KICK[(int)from][(int)to];
	}
	else
	{
		kick = JLSTZ_KICK[(int)from][(int)to];
	}

	for (int i = 0; i < 5; i++)
	{
		Vec2i testPos =
			originalPos + kick[i];

		if (IsMoveValid(testPos))
		{
			currentMino->SetGridPosition(testPos);
			lastKickIndex = i; // 最後に成功したキックの番号を保存
			return;
		}
	}

	// 全失敗
	currentMino->RotateRightRaw();
	currentMino->SetGridPosition(originalPos);
}
#pragma endregion

bool MinoManager::IsMoveValid(const Vec2i& newPos) const
{
	for (int i = 0; i < 4; ++i)
	{
		Vec2i local = currentMino->GetBlockLocalPosition(i);

		Vec2i world =
		{
			newPos.x + local.x,
			newPos.y + local.y
		};

		// 左右壁
		if (world.x < 0 ||
			world.x >= Config::FIELD_WIDTH)
		{
			return false;
		}

		// 下
		if (world.y >= Config::FIELD_HEIGHT)
		{
			return false;
		}

		// 固定ブロック
		if (world.y >= 0)
		{
			if (gameMap->GetBlock(world) != nullptr)
			{
				return false;
			}
		}
	}

	return true;
}

bool MinoManager::CheckTSpinCondition(int& outCornerCount, bool& outIsMini)
{
	if (currentMino->GetType() != MinoType::T)
	{
		return false;
	}

	if (!lastActionIsRotate)
	{
		return false;
	}

	Vec2i center = currentMino->GetGridPosition();

	Vec2i corners[4] =
	{
		{-1,-1},
		{ 1,-1},
		{-1, 1},
		{ 1, 1}
	};

	int blockCount = 0;

	for (auto& offset : corners)
	{
		Vec2i world =
			center + offset;

		if (IsCornerFilled(world))
		{
			blockCount++;
		}
	}

	if (blockCount < 3)
	{
		return false;
	}

	static constexpr Vec2i frontCorners[4][2] =
	{
		// UP
		{
			{-1,-1},
			{ 1,-1}
		},

		// RIGHT
		{
			{1,-1},
			{1, 1}
		},

		// DOWN
		{
			{-1,1},
			{ 1,1}
		},

		// LEFT
		{
			{-1,-1},
			{-1, 1}
		}
	};

	RotateState rot =
		currentMino->GetRotateState();

	int frontCount = 0;

	for (int i = 0; i < 2; i++)
	{
		Vec2i world =
			center
			+ frontCorners[(int)rot][i];

		if (IsCornerFilled(world))
		{
			frontCount++;
		}
	}

	bool proper =
		(frontCount == 2)
		|| (lastKickIndex == 3)
		|| (lastKickIndex == 4);

	outIsMini = !proper;

	outCornerCount = blockCount;
	isEffectPlaying = true;
	return true;
}

bool MinoManager::IsCornerFilled(const Vec2i& pos) const
{
	// 壁・床
	if (pos.x < 0
		|| pos.x >= Config::FIELD_WIDTH
		|| pos.y >= Config::FIELD_HEIGHT)
	{
		return true;
	}

	// 上側は空扱い
	if (pos.y < 0)
	{
		return false;
	}

	// 固定ブロック
	return gameMap->GetBlock(pos) != nullptr;
}
void MinoManager::AddScore(int lineCount, bool isTspin, bool isMini)
{
	int baseScore = 0;

	if (isTspin)
	{
		if (isMini)
		{
			if (lineCount == 0) baseScore = 100;
			else if (lineCount == 1) baseScore = 200;
			else if (lineCount == 2) baseScore = 400;
		}
		else
		{
			if (lineCount == 0) baseScore = 40000;
			else if (lineCount == 1) baseScore = 80000;
			else if (lineCount == 2) baseScore = 120000;
			else if (lineCount == 3) baseScore = 160000;
		}
	}
	else
	{
		if (lineCount == 1) baseScore = 100;
		else if (lineCount == 2) baseScore = 300;
		else if (lineCount == 3) baseScore = 500;
		else if (lineCount == 4) baseScore = 800;
	}

	score += baseScore;
	totalLinesCleared += lineCount;

	if (isTspin) {
		isEffectPlaying = true;
	}

	if (lineCount > 0)
	{
		linesForNextLevel -= lineCount;

		while (linesForNextLevel <= 0)
		{
			level++;
			linesForNextLevel += 10;
		}

		UpdateFallInterval();
	}
}

void MinoManager::UpdateFallInterval()
{
	int tableIndex = level - 1;
	if (tableIndex < 0) tableIndex = 0;
	if (tableIndex > 15) tableIndex = 15;

	fallInterval = SPEED_TABLE[tableIndex];
}

int MinoManager::CalculateAttack(int lineCount, bool isTspin, bool isMini)
{
	int attack = 0;

	// TSpin
	if (isTspin)
	{
		if (isMini)
		{
			if (lineCount == 1)
			{
				attack = 1;
			}
		}
		else
		{
			switch (lineCount)
			{
			case 1:
				attack = 2; 
				break;
			case 2:
				attack = 4;
				break;
			case 3:
				attack = 6;
				break;
			default:
				break;
			}
		}
	}
	else
	{
		switch (lineCount)
		{
		case 2:
			attack = 1;
			break;
		case 3:
			attack = 2;
			break;
		case 4:
			attack = 4;
			break;
		default:
			break;
		}
	}

	//B2B(強い攻撃の二連続).
	bool isB2BAction =
		(lineCount == 4)
		|| (isTspin && lineCount > 0);

	if (isB2BAction)
	{
		if (backToBack)
		{
			attack += 1;
		}

		backToBack = true;
	}
	else if (lineCount > 0)
	{
		backToBack = false;
	}

	//連続消しでの攻撃力増加.
	if (lineCount > 0)
	{
		ren++;

		if (ren >= 1)
		{
			attack += ren / 2;
		}
	}
	else
	{
		ren = -1;
	}

	return attack;
}

void MinoManager::ApplyGarbage(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		gameMap->AddGarbageLine(boardOrigin);
	}
}

TetrisData MinoManager::GetTetrisDataH()
{
	TetrisData data;
	data.currentMinoX = currentMino->GetGridPosition().x;
	data.currentMinoY = currentMino->GetGridPosition().y;
	data.currentMinoType = Cast<int>(currentMino->GetType());
	data.currentMinoRotateState = Cast<int>(currentMino->GetRotateState());
	data.nextMinoType = Cast<int>(nextMino->GetType());
	data.holdMinoType = Cast<int>(holdMinoType);
	for (int j = 0; j < Config::FIELD_HEIGHT; j++)
	{
		for (int i = 0; i < Config::FIELD_WIDTH; i++)
		{
			Block* block = gameMap->GetBlock(Vec2i{ i, j });
			if (block)
			{
				data.map[j][i] = Cast<int>(block->GetColor());
			}
			else
			{
				data.map[j][i] = -1;
			}
		}
	}

	int blockCount = 0;

	for (int y = 0; y < Config::FIELD_HEIGHT; y++)
	{
		for (int x = 0; x < Config::FIELD_WIDTH; x++)
		{
			if (data.map[y][x] != -1)
			{
				blockCount++;
			}
		}
	}

	//printfDx(L"Send Block Count = %d\n", blockCount);
	data.isFixed = isFixed;

	return data;
}