#include "stdafx.h"
#include "GameMap.h"
#include "GameObjectManager.h"

GameMap::GameMap()
{
}

GameMap::~GameMap()
{
}

void GameMap::Initialize()
{
}

void GameMap::Finalize()
{
}

void GameMap::Update()
{

}

void GameMap::Draw()
{
}

int GameMap::BreakBlockCheck()
{
	int lineCount = 0;

	for (int j = 0; j < Config::FIELD_HEIGHT; j++)
	{
		bool isFull = true;

		for (int i = 0; i < Config::FIELD_WIDTH; i++)
		{
			if (map[i][j] == nullptr)
			{
				isFull = false;
				break;
			}
		}

		if (isFull)
		{
			BreakLine(j);
			lineCount++;
			j--; //ラインが消えたので、同じ行をもう一度チェックするためにjをデクリメント.
		}
	}
	return lineCount;
}


void GameMap::BreakLine(int line)
{
	for (int i = 0; i < Config::FIELD_WIDTH; i++)
	{
		if (map[i][line])
		{
			map[i][line]->Destroy();
			map[i][line] = nullptr;
		}
	}
	for (int j = line - 1; j >= 0; j--)
	{
		for (int i = 0; i < Config::FIELD_WIDTH; i++)
		{
			if (map[i][j])
			{
				map[i][j]->SetGridPosition({ i, j + 1 });
				map[i][j + 1] = map[i][j];
				map[i][j] = nullptr;
			}
		}
	}
}

void GameMap::SetBlock(Tetromino* mino)
{
	for (int i = 0; i < 4; ++i)
	{
		Vec2i pos = mino->GetBlockGridPosition(i);
		map[pos.x][pos.y] = mino->GetBlock(i);
	}
	mino->Destroy();
}

void GameMap::AddGarbageLine(Vec2f boardVec)
{

	if (garbageHoleCounter <= 0)
	{
		garbageHoleX =
			MyStd::IRange(0, Config::FIELD_WIDTH - 1);

		garbageHoleCounter =
			MyStd::IRange(2, 5);
	}
	garbageHoleCounter--;

	for (int y = 0;y < Config::FIELD_HEIGHT - 1;y++)
	{
		for (int x = 0;
			x < Config::FIELD_WIDTH;
			x++)
		{
			map[x][y] = map[x][y + 1];

			if (map[x][y] != nullptr)
			{
				map[x][y]->SetGridPosition(
					Vec2i{ x, y });
			}

		}
	}

	for (int x = 0;
		x < Config::FIELD_WIDTH;
		x++)
	{
		if (x == garbageHoleX)
		{
			map[x][Config::FIELD_HEIGHT - 1] = nullptr;
		}
		else
		{
			auto* block =
				GameObjectManager::GetInstance()
				.Create<Block>(BlockColor::WHITE, DrawType::Normal, boardVec);

			block->SetGridPosition(
				Vec2i{x, Config::FIELD_HEIGHT - 1
				});

			map[x][Config::FIELD_HEIGHT - 1]
				= block;
		}
	}
}
