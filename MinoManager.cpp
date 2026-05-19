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
	nextMino->SetGridPosition(Vec2i{ Config::FIELD_WIDTH + 2, 2 }); //ƒtƒB[ƒ‹ƒh‚Ì‰E‘¤‚ÉŸ‚Ìƒ~ƒm‚ğ•\¦‚·‚é‚½‚ß‚ÌˆÊ’u.

	gameMap = std::make_unique<GameMap>();
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
	auto& input = InputManager::GetInstance();
	auto Gpos = currentMino->GetGridPosition();
	if (input.isTrigger(KEY_INPUT_E))
	{
		TryRotateRight();
	}
	else if (input.isTrigger(KEY_INPUT_Q))
	{
		TryRotateLeft();
	}
	else if (input.isTrigger(KEY_INPUT_A))
	{
		TestMino(Vec2i(-1, 0));
	}
	else if (input.isTrigger(KEY_INPUT_D))
	{
		TestMino(Vec2i(1, 0));
	}
	else if (input.isTrigger(KEY_INPUT_S))
	{
		TestMino(Vec2i(0, 1));
	}
	else if (input.isTrigger(KEY_INPUT_W))
	{
		HardDrop();
	}
	
	if (fallTimer >= fallInterval)
	{
		TestMino(Vec2i(0, 1));
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

	// ŒÅ’è
	LockMino();
}

void MinoManager::LockMino()
{
	gameMap->SetBlock(currentMino);
	nextMino->SetGridPosition(Vec2i{ 5, 1 }); //Ÿ‚Ìƒ~ƒm‚ğƒtƒB[ƒ‹ƒh‚Ìã‚Éo‚·‚½‚ß‚ÌˆÊ’u.
	currentMino = nextMino;
	nextMino =GameObjectManager::GetInstance().Create<Tetromino>(GetNextType());
	nextMino->SetGridPosition(Vec2i{ Config::FIELD_WIDTH + 2, 2 }); //ƒtƒB[ƒ‹ƒh‚Ì‰E‘¤‚ÉŸ‚Ìƒ~ƒm‚ğ•\¦‚·‚é‚½‚ß‚ÌˆÊ’u.
}

#pragma region ‰ñ“]•”•ªSRS

static constexpr Vec2i JLSTZ_KICK[4][4][5] =
{
	// UP (0)
	{
		{}, // UP->UP

		// UP -> RIGHT (0 -> 1)
		{
			{0,0},
			{-1,0},
			{-1,-1}, // Œ³ 1 ¨ C³ -1 (ã‚ÖƒLƒbƒN)
			{0,2},   // Œ³ -2 ¨ C³ 2 (‰º‚ÖƒLƒbƒN)
			{-1,2},  // Œ³ -2 ¨ C³ 2
		},

		{},

		// UP -> LEFT (0 -> 3)
		{
			{0,0},
			{1,0},
			{1,-1}, // Œ³ 1 ¨ C³ -1
			{0,2},  // Œ³ -2 ¨ C³ 2
			{1,2},  // Œ³ -2 ¨ C³ 2
		}
	},

	// RIGHT (1)
	{
		// RIGHT -> UP (1 -> 0)
		{
			{0,0},
			{1,0},
			{1,1},   // Œ³ -1 ¨ C³ 1
			{0,-2},  // Œ³ 2 ¨ C³ -2
			{1,-2},  // Œ³ 2 ¨ C³ -2
		},

		{},

		// RIGHT -> DOWN (1 -> 2)
		{
			{0,0},
			{1,0},
			{1,1},   // Œ³ -1 ¨ C³ 1
			{0,-2},  // Œ³ 2 ¨ C³ -2
			{1,-2},  // Œ³ 2 ¨ C³ -2
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
			{-1,-1}, // Œ³ 1 ¨ C³ -1
			{0,2},   // Œ³ -2 ¨ C³ 2
			{-1,2},  // Œ³ -2 ¨ C³ 2
		},

		{},

		// DOWN -> LEFT (2 -> 3)
		{
			{0,0},
			{1,0},
			{1,-1},  // Œ³ 1 ¨ C³ -1
			{0,2},   // Œ³ -2 ¨ C³ 2
			{1,2},   // Œ³ -2 ¨ C³ 2
		}
	},

	// LEFT (3)
	{
		// LEFT -> UP (3 -> 0)
		{
			{0,0},
			{-1,0},
			{-1,1},  // Œ³ -1 ¨ C³ 1
			{0,-2},  // Œ³ 2 ¨ C³ -2
			{-1,-2}, // Œ³ 2 ¨ C³ -2
		},

		{},

		// LEFT -> DOWN (3 -> 2)
		{
			{0,0},
			{-1,0},
			{-1,1},  // Œ³ -1 ¨ C³ 1
			{0,-2},  // Œ³ 2 ¨ C³ -2
			{-1,-2}, // Œ³ 2 ¨ C³ -2
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
			return;
		}
	}

	// ‘S¸”s
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
			return;
		}
	}

	// ‘S¸”s
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

		// ¶‰E•Ç
		if (world.x < 0 ||
			world.x >= Config::FIELD_WIDTH)
		{
			return false;
		}

		// ‰º
		if (world.y >= Config::FIELD_HEIGHT)
		{
			return false;
		}

		// ŒÅ’èƒuƒƒbƒN
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
