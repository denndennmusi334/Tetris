#include "stdafx.h"
#include "Block.h"

using namespace MyStd;

Block::Block()
{

}

Block::Block(BlockColor _color) : color(_color)
{
}

Block::~Block()
{

}

void Block::Initialize()
{
	grHandle = ImageLoadManager::GetInstance().getGraphHandle(Cast<E_TEXTURE>(color));
}

void Block::Draw(const Camera& camera)
{
	DrawRotaGraph(Cast<int>(GetPosition().x), Cast<int>(GetPosition().y), 1.0f, 0.0f, grHandle, TRUE);
	//animator.Draw();
}


void Block::SetGridPosition(const Vec2i& pos)
{
	Gpos = pos;
	SetPosition(Vec2f{ Gpos.x * Config::BLOCK_SIZE + Config::BLOCK_SIZE / 2.0f + Config::FIELD_X, Gpos.y * Config::BLOCK_SIZE + Config::BLOCK_SIZE / 2.0f });
}
