#include "stdafx.h"
#include "MinoManager.h"
#include "InputManager.h"
#include "GameObjectManager.h"
#include "TimeManager.h"

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
	currentMino = GameObjectManager::GetInstance().Create<Tetromino>(GetNextType());

	nextMino = GameObjectManager::GetInstance().Create<Tetromino>(GetNextType());
	nextMino->SetGridPosition(Vec2i{ Config::FIELD_WIDTH + 2, 2 }); //フィールドの右側に次のミノを表示するための位置.

	gameMap = std::make_unique<GameMap>();

	level = 1;
	linesForNextLevel = 10;
	UpdateFallInterval();
}

void MinoManager::Finalize()
{
}

void MinoManager::Update()
{
	MinoUpdate();
}

void MinoManager::Draw(const Camera& camera)
{
}

void MinoManager::MinoUpdate()
{
	if (isGameOver)return;

	auto& input = InputManager::GetInstance();
	auto Gpos = currentMino->GetGridPosition();
	if (input.isTrigger(KEY_INPUT_E))
	{
		lockTimer = 0.0f;
		TryRotateRight();
		lastActionIsRotate = true;
	}
	else if (input.isTrigger(KEY_INPUT_Q))
	{
		lockTimer = 0.0f;
		TryRotateLeft();
		lastActionIsRotate = true;
	}
	else if (input.isTrigger(KEY_INPUT_A))
	{
		lockTimer = 0.0f;
		TestMino(Vec2i(-1, 0));
		lastActionIsRotate = false;
	}
	else if (input.isTrigger(KEY_INPUT_D))
	{
		lockTimer = 0.0f;
		TestMino(Vec2i(1, 0));
		lastActionIsRotate = false;
	}
	else if (input.isTrigger(KEY_INPUT_S))
	{
		lockTimer = 0.0f;
		TestMino(Vec2i(0, 1));
		lastActionIsRotate = false;
	}
	else if (input.isTrigger(KEY_INPUT_SPACE))
	{
		HardDrop();
	}
	else if (input.isTrigger(KEY_INPUT_LSHIFT))
	{
		TryHold();
	}
	
	if (fallTimer >= fallInterval)
	{
		TestMino(Vec2i(0, 1));
		lastActionIsRotate = false;
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

void MinoManager::TryHold()
{
	if (hasHeldInThisTurn) return;

	MinoType currentType = currentMino->GetType();

	if (holdMinoType == MinoType::None)
	{
		holdMinoType = currentType;
		holdMinoVisual = currentMino;
		currentMino = nextMino;
		currentMino->SetGridPosition(Vec2i{ 5, 0 });

		nextMino = GameObjectManager::GetInstance().Create<Tetromino>(GetNextType());
		nextMino->SetGridPosition(Vec2i{ Config::FIELD_WIDTH + 2, 2 });
	}
	else
	{
		MinoType prevHoldType = holdMinoType;
		holdMinoType = currentType;

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

void MinoManager::TestMino(const Vec2i& newPos) const
{
	Vec2i testPos = currentMino->GetGridPosition() + newPos;
	if (IsMoveValid(testPos)) currentMino->SetGridPosition(testPos);
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

	int cornerCount = 0;
	bool isMini = false;
	bool isTspin = CheckTSpinCondition(cornerCount, isMini);

	int clearedLines = gameMap->BreakBlockCheck();

	AddScore(clearedLines, isTspin, isMini);

	nextMino->SetGridPosition(Vec2i{ 5, 0 });
	currentMino = nextMino;
	nextMino = GameObjectManager::GetInstance().Create<Tetromino>(GetNextType());
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
	if (currentMino->GetType() != MinoType::T) return false;

	if (!lastActionIsRotate) return false;

	Vec2i center = currentMino->GetGridPosition(); 

	Vec2i corners[4] = {
		{ -1, -1 }, { 1, -1 },  // 左上、右上
		{ -1,  1 }, { 1,  1 }   // 左下、右下
	};

	int blockCount = 0;
	for (const auto& offset : corners)
	{
		Vec2i world = center + offset;

		if (world.x < 0 || world.x >= Config::FIELD_WIDTH || world.y >= Config::FIELD_HEIGHT)
		{
			blockCount++;
		}
		else if (world.y >= 0 && gameMap->GetBlock(world) != nullptr)
		{
			blockCount++;
		}
	}

	if (blockCount >= 3)
	{
		outCornerCount = blockCount;


		if (lastKickIndex == 3 || lastKickIndex == 4)
		{
			outIsMini = false; 
		}
		else
		{
			outIsMini = true;
		}
		return true;
	}

	return false;
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
