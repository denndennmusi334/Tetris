#include "stdafx.h"
#include "GameMap.h"

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
