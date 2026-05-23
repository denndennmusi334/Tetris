#include "stdafx.h"
#include "Tetromino.h"
#include "GameObjectManager.h"

using namespace MyStd;

Tetromino::Tetromino()
{
}

Tetromino::Tetromino(MinoType _type)
	: type(_type)
{
}

Tetromino::~Tetromino()
{
}

void Tetromino::Initialize()
{

	Gpos = Vec2i{ Config::FIELD_WIDTH / 2, 1 };

	for (int i = 0; i < 4; ++i)
	{
		blocks[i] = GameObjectManager::GetInstance().Create<Block>(SetColor(type));
		blocks[i]->SetGridPosition(GetBlockGridPosition(i));
	}

}

void Tetromino::Finalize()
{

}

void Tetromino::Update()
{
}

void Tetromino::Draw(const Camera& camera)
{

}

Vec2i Tetromino::GetBlockLocalPosition(int i) const
{
	static constexpr Vec2i SHAPES[7][4][4] =
	{
		// =====================
		// I
		// =====================
	{
		// UP
		{
				{-2,0}, {-1,0}, {0,0}, {1,0}
		},
		// RIGHT
		{
			{0,-1}, {0,0}, {0,1}, {0,2}
		},
		// DOWN
		{
			{-2,1}, {-1,1}, {0,1}, {1,1}
		},
		// LEFT
		{
			{-1,-1}, {-1,0}, {-1,1}, {-1,2}
		}
	},
		// =====================
		// J
		// =====================
		{
		// UP
		{
			{-1,-1}, {-1,0}, {0,0}, {1,0}
		},
		// RIGHT
		{
			{0,-1}, {0,0}, {0,1}, {1,-1}
		},
		// DOWN
		{
			{-1,0}, {0,0}, {1,0}, {1,1}
		},
		// LEFT
		{
			{-1,1}, {0,-1}, {0,0}, {0,1}
		}
	},
		// =====================
		// L
		// =====================
		{
		// UP
		{
			{-1,0}, {0,0}, {1,0}, {1,-1}
		},
		// RIGHT
		{
			{0,-1}, {0,0}, {0,1}, {1,1}
		},
		// DOWN
		{
			{-1,1}, {-1,0}, {0,0}, {1,0}
		},
		// LEFT
		{
			{-1,-1}, {0,-1}, {0,0}, {0,1}
		}
	},
		// =====================
		// O
		// =====================
		{
		// UP
		{
			{0,0}, {1,0}, {0,1}, {1,1}
		},
		// RIGHT
		{
			{0,0}, {1,0}, {0,1}, {1,1}
		},
		// DOWN
		{
			{0,0}, {1,0}, {0,1}, {1,1}
		},
		// LEFT
		{
			{0,0}, {1,0}, {0,1}, {1,1}
		}
	},

		// =====================
		// S
		// =====================
		{
		// UP
		{
			{0,0}, {1,0}, {-1,1}, {0,1}
		},
		// RIGHT
		{
			{0,-1}, {0,0}, {1,0}, {1,1}
		},
		// DOWN
		{
			{0,0}, {1,0}, {-1,1}, {0,1}
		},
		// LEFT
		{
			{0,-1}, {0,0}, {1,0}, {1,1}
		}
	},

		// =====================
		// Z
		// =====================
		{
		// UP
		{
			{-1,0}, {0,0}, {0,1}, {1,1}
		},
		// RIGHT
		{
			{1,-1}, {0,0}, {1,0}, {0,1}
		},
		// DOWN
		{
			{-1,0}, {0,0}, {0,1}, {1,1}
		},
		// LEFT
		{
			{1,-1}, {0,0}, {1,0}, {0,1}
		}
	},

		// =====================
		// T
		// =====================
		{
		// UP
		{
			{-1,0}, {0,0}, {1,0}, {0,-1}
		},
		// RIGHT
		{
			{0,-1}, {0,0}, {0,1}, {1,0}
		},
		// DOWN
		{
			{-1,0}, {0,0}, {1,0}, {0,1}
		},
		// LEFT
		{
			{0,-1}, {0,0}, {0,1}, {-1,0}
		}
	}
	};
	return SHAPES[(int)type][(int)rotateState][i];
}

BlockColor Tetromino::SetColor(MinoType _type)
{
	BlockColor color = BlockColor::COLOR_NULL;
	switch (_type)
	{
	case MinoType::I:
		color = BlockColor::LIGHT_BLUE;
		break;
	case MinoType::J:
		color = BlockColor::BLUE;
		break;
	case MinoType::L:
		color = BlockColor::ORANGE;
		break;
	case MinoType::O:
		color = BlockColor::YELLOW;
		break;
	case MinoType::S:
		color = BlockColor::GREEN;
		break;
	case MinoType::T:
		color = BlockColor::PURPLE;
		break;
	case MinoType::Z:
		color = BlockColor::RED;
		break;
	default:
		assert(false && "Invalid MinoType");
	}
	return color;
}

void Tetromino::SetBlockPos()
{
	for (int i = 0; i < 4; ++i)
	{
		if (blocks[i])
		{
			blocks[i]->SetGridPosition(GetBlockGridPosition(i));
		}
	}
}

void Tetromino::RotateRightRaw()
{
	rotateState = Cast<RotateState>(((int)rotateState + 1) % 4);
}

void Tetromino::RotateLeftRaw()
{
	rotateState = Cast<RotateState>(((int)rotateState + 3) % 4);
}
