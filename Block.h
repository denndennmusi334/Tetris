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

	COLOR_NULL,
};

class Block :
    public GameObject
{
private:
	Vec2i Gpos = { 0,0 };
	BlockColor color = BlockColor::COLOR_NULL;
	int grHandle = -1;

	bool isBreaking = false; //ブロックが壊れているかどうか.
public:
	Block();
	Block(BlockColor _color);
	~Block();
	void Initialize() override;
	void Finalize() override {};
	void Update() override {};
	void Draw(const Camera& camera) override;
	//ブロックが壊れるときに呼ぶ関数.
	void Break();
	bool isBreakingBlock() const { return isBreaking; }

	void SetGridPosition(const Vec2i& pos);
	Vec2i GetGridPosition() const { return Gpos; }

};

