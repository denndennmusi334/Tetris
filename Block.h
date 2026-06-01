#pragma once
#include "GameObject.h"

enum class BlockColor
{
	LIGHT_BLUE,
	BLUE,
	ORANGE,
	YELLOW,
	GREEN,
	PURPLE,
	RED,
	WHITE,

	COLOR_NULL,
};

enum class DrawType
{
	Normal,
	Ghost
};

class Block :
    public GameObject
{
private:
	Vec2i Gpos = { 0,0 };
	BlockColor color = BlockColor::COLOR_NULL;
	int grHandle = -1;
	DrawType drawType = DrawType::Normal;

	Vec2f boardOrigin = { Config::FIELD_X, Config::FIELD_Y };

public:
	Block();
	Block(BlockColor _color, DrawType _drawType, Vec2f boardVec);
	~Block();
	void Initialize() override;
	void Finalize() override {};
	void Update() override {};
	void Draw(const Camera& camera) override;
	//ブロックが壊れるときに呼ぶ関数.

	void SetColor(BlockColor _color);
	void SetGridPosition(const Vec2i& pos);
	Vec2i GetGridPosition() const { return Gpos; }

};

