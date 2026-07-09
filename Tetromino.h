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
	T,
	None
};



class Tetromino : public GameObject
{
private:
	Vec2i Gpos = { 0,0 };

	Vec2i BlockLocalPos[4] = { {0,0}, {0,0}, {0,0}, {0,0} };

	Block* blocks[4] = { nullptr };
	MinoType type = MinoType::I;
	DrawType drawType = DrawType::Normal;

	RotateState rotateState = RotateState::UP;

	void SetBlockPos();
	BlockColor SetColor(MinoType _type);
	Vec2f boardOrigin = { Config::FIELD_X, Config::FIELD_Y };
public:
	Tetromino();
	Tetromino(MinoType type, Vec2f boardVec, DrawType drawType = DrawType::Normal);
	~Tetromino();
#pragma region inline関数

	void SetType(MinoType _type) { type = _type; 
	SetColor(SetColor(type));
	for (int i = 0; i < 4; ++i)
	{
		if (blocks[i])
		{
			blocks[i]->SetGridPosition(GetBlockGridPosition(i));
		}
	}
	}

	MinoType GetType() const { return type; }
	Vec2i GetGridPosition() const { return Gpos; }
	
	RotateState GetRotateState() const { return rotateState; }
	void SetRotateState(RotateState state) { 
		rotateState = state; 
		for (int i = 0; i < 4; ++i)
		{
			if (blocks[i])
			{
				blocks[i]->SetGridPosition(GetBlockGridPosition(i));
			}
		}

	}
	
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

	void AllDestroyBlock()
	{
		for (int i = 0; i < 4; ++i)
		{
			if (blocks[i])
			{
				blocks[i]->Destroy();
				blocks[i] = nullptr;
			}
		}
		Destroy();
	}

	void CopyTransFrom(const Tetromino& other)
	{
		SetGridPosition(other.GetGridPosition());
		type = other.GetType();
		SetColor(SetColor(type));
		rotateState = other.rotateState;
		SetBlockPos();
	}

	void SetColor(BlockColor color)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (blocks[i])
			{
				blocks[i]->SetColor(color);
			}
		}
	}
#pragma endregion

#pragma region 回転.
	void RotateRightRaw();
	void RotateLeftRaw();
#pragma endregion

	void Initialize();
	void Finalize();
	void Update();
	void Draw(const Camera& camera);
};

