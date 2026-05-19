#pragma once
#include "Job.h"
#include "Block.h"
#include "Tetromino.h"

class GameMap :
    public Job
{
private:
	Block* map[Config::FIELD_WIDTH][Config::FIELD_HEIGHT] = { nullptr };
	int score = 0;
public:
	GameMap();
	~GameMap();

	void BreakBlockCheck();
	void BreakLine(int line);

	void SetBlock(Tetromino* mino);
	Block* GetBlock(Vec2i pos)
	{
		assert(this);
		if (pos.x < 0 || pos.x >= Config::FIELD_WIDTH)
		{
			return nullptr;
		}

		if (pos.y < 0 || pos.y >= Config::FIELD_HEIGHT)
		{
			return nullptr;
		}
		return map[pos.x][pos.y];
	}

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};

