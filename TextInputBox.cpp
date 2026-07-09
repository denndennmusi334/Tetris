#include "stdafx.h"
#include "TextInputBox.h"
#include "InputManager.h"

using namespace MyStd;

void TextInputBox::Draw(const Camera& camera)
{
	if (!isActive) return;
	unsigned int color = isUse ? selectColor : idleColor;
	const Vec2f& pos = GetPosition();
	DrawBox(
		ICast(pos.x), ICast(pos.y),
		ICast(pos.x + width), ICast(pos.y + height),
		color, TRUE);
	DrawString(ICast(pos.x), ICast(pos.y), MyStd::Utf8ToWstring(text).c_str(), 0x00000000);
}

void TextInputBox::Update()
{
	if (!isActive) return;
	isUse = IsActiveCheck();
	if (!isUse) return;
	int input = GetInputChar(TRUE);
	if (input == 0) return;
	if (input == '\b') // バックスペース
	{
		if (!text.empty())
			text.pop_back();
	}
	else if (input == '\r' || input == '\n') 
	{
		isUse = false; // 入力完了
	}
	else if (input >= 32 && input <= 126) 
	{
		text += MyStd::Cast<char>(input);
	}
}

bool TextInputBox::IsActiveCheck() const
{
	if (isUse)return true;
	auto& iMgr = InputManager::GetInstance();
	if (iMgr.IsMouseTriggerLeft())
	{
		Vec2i mousePos = iMgr.GetMousePosition();
		Vec2f pos = GetPosition();
		if (
			mousePos.x >= pos.x && mousePos.x <= pos.x + width &&
			mousePos.y >= pos.y && mousePos.y <= pos.y + height)
		{
			while (GetInputChar(TRUE) != 0) {}
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}