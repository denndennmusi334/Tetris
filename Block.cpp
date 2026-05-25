#include "stdafx.h"
#include "Block.h"

using namespace MyStd;

Block::Block()
{

}

Block::Block(BlockColor _color, DrawType _drawType, Vec2f boardVec)
	: color(_color), drawType(_drawType), boardOrigin(boardVec)
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
	if (drawType == DrawType::Ghost) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128); // 半透明にする
	DrawRotaGraph(Cast<int>(GetPosition().x), Cast<int>(GetPosition().y), 1.0f, 0.0f, grHandle, TRUE);
	//animator.Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドモードを元に戻す
}

void Block::SetColor(BlockColor _color)
{
	color = _color;
	grHandle = ImageLoadManager::GetInstance().getGraphHandle(Cast<E_TEXTURE>(color));
}

void Block::SetGridPosition(const Vec2i& pos)
{
	Gpos = pos;
	SetPosition(Vec2f{ Gpos.x * Config::BLOCK_SIZE + Config::BLOCK_SIZE / 2.0f + boardOrigin.x ,Gpos.y * Config::BLOCK_SIZE + Config::BLOCK_SIZE / 2.0f});
}
