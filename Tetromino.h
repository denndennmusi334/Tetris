#pragma once
#include "Block.h"

enum class RotateState
{
	UP,
	RIGHT,
	DOWN,
	LEFT,
};

enum class MinoType
{
	I,
	J,
	L,
	O,
	S,
	Z,
	T
};

class Tetromino : public GameObject
{
private:
	Vec2i Gpos = { 0,0 };

	Vec2i BlockLocalPos[4] = { {0,0}, {0,0}, {0,0}, {0,0} };

	Block* blocks[4] = { nullptr };
	MinoType type = MinoType::I;

	RotateState rotateState = RotateState::UP;

	void SetBlockPos();
	BlockColor SetColor(MinoType _type);
public:
	Tetromino();
	Tetromino(MinoType type);
	~Tetromino();
#pragma region inlineä÷êî


	MinoType GetType() const { return type; }
	Vec2i GetGridPosition() const { return Gpos; }
	
	RotateState GetRotateState() const { return rotateState; }
	
	void SetGridPosition(const Vec2i& pos)
	{
		Gpos = pos;
		for (int i = 0; i < 4; ++i)
		{
			if (blocks[i])
			{
				blocks[i]->SetGridPosition(GetBlockGridPosition(i));
			}
		}
	}

	Vec2i GetBlockLocalPosition(int i) const;

	Vec2i GetBlockGridPosition(int index) const
	{
		if (index < 0 || index >= 4) return { 0,0 };
		return Gpos + GetBlockLocalPosition(index);
	}

	Block* GetBlock(int index) const
	{
		if (index < 0 || index >= 4) return nullptr;
		return blocks[index];
	}
#pragma endregion

#pragma region âÒì].
	void RotateRightRaw();
	void RotateLeftRaw();
#pragma endregion

	void Initialize();
	void Finalize();
	void Update();
	void Draw(const Camera& camera);
};

