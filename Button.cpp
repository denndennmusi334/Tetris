#include "stdafx.h"
#include "Button.h"
#include "InputManager.h"

constexpr float HANDLE_SIZE = 6.0f;
constexpr float MIN_SIZE = 4.0f;

Button::Button(const ButtonData& data)
    : bData(data)
{
    RebuildFont();
}

void Button::Initialize()
{
}

void Button::Finalize()
{
    if (fontHandle != -1)
    {
        DeleteFontToHandle(fontHandle);
        fontHandle = -1;
    }
}

void Button::Draw(const Camera& camera)
{
	if (!isActive)return;
    unsigned int color = bData.neutral_color;
    if (isOn)color = bData.on_color;
    if (bData.button_type == BUTTON_TYPE::SQUARE)
    {
        DrawBoxAA(
            bData.pos.x - bData.width / 2,
            bData.pos.y - bData.height / 2,
            bData.pos.x + bData.width / 2,
            bData.pos.y + bData.height / 2,
            color,
            bData.fill_flag
        );
    }
    else
    {
        DrawOvalAA(
            bData.pos.x,
            bData.pos.y,
            bData.width / 2,
            bData.height / 2,
            32,
            color,
            bData.fill_flag
        );
    }

    if (bData.grH == -1 && !bData.text.empty())
    {
        int w = GetDrawStringWidthToHandle(
            bData.text.c_str(),
            (int)bData.text.length(),
            fontHandle
        );
        int h = GetFontSizeToHandle(fontHandle);

        DrawStringToHandle(
            (int)(bData.pos.x - w / 2),
            (int)(bData.pos.y - h / 2),
            bData.text.c_str(),
            bData.text_color,
            fontHandle
        );
    }
    if (bData.grH != -1)
    {
		printfDx(L"DrawGraphToHandle : %d\n", bData.grH);
    }
}

BOOL Button::HitCheck(float mx, float my) const
{
    float l = bData.pos.x - bData.width / 2;
    float r = bData.pos.x + bData.width / 2;
    float t = bData.pos.y - bData.height / 2;
    float b = bData.pos.y + bData.height / 2;

    return (mx >= l && mx <= r && my >= t && my <= b);
}


#if !USE_BUTTON_EDIT
void Button::Update()
{
    if (!isActive)return;
    auto& iMgr = InputManager::GetInstance();
    float mx = (float)iMgr.GetMouseX();
    float my = (float)iMgr.GetMouseY();

    isOn = HitCheck(mx, my);

    isPush = isOn && iMgr.IsMouseReleaseLeft();
}
#else
void Button::Update(float mx, float my)
{
    float dx = mx - startMX;
    float dy = my - startMY;

    switch (resizeDir)
    {
    case ResizeDir::NONE:
        bData.pos.x = startX + dx;
        bData.pos.y = startY + dy;
        break;

    case ResizeDir::LEFT:
        bData.width = std::max(MIN_SIZE, startW - dx);
        bData.pos.x = startX + dx / 2;
        break;

    case ResizeDir::RIGHT:
        bData.width = std::max(MIN_SIZE, startW + dx);
        bData.pos.x = startX + dx / 2;
        break;

    case ResizeDir::TOP:
        bData.height = std::max(MIN_SIZE, startH - dy);
        bData.pos.y = startY + dy / 2;
        break;

    case ResizeDir::BOTTOM:
        bData.height = std::max(MIN_SIZE, startH + dy);
        bData.pos.y = startY + dy / 2;
        break;

    case ResizeDir::TOP_LEFT:
        bData.width = std::max(MIN_SIZE, startW - dx);
        bData.height = std::max(MIN_SIZE, startH - dy);
        bData.pos.x = startX + dx / 2;
        bData.pos.y = startY + dy / 2;
        break;

    case ResizeDir::TOP_RIGHT:
        bData.width = std::max(MIN_SIZE, startW + dx);
        bData.height = std::max(MIN_SIZE, startH - dy);
        bData.pos.x = startX + dx / 2;
        bData.pos.y = startY + dy / 2;
        break;

    case ResizeDir::BOTTOM_LEFT:
        bData.width = std::max(MIN_SIZE, startW - dx);
        bData.height = std::max(MIN_SIZE, startH + dy);
        bData.pos.x = startX + dx / 2;
        bData.pos.y = startY + dy / 2;
        break;

    case ResizeDir::BOTTOM_RIGHT:
        bData.width = std::max(MIN_SIZE, startW + dx);
        bData.height = std::max(MIN_SIZE, startH + dy);
        bData.pos.x = startX + dx / 2;
        bData.pos.y = startY + dy / 2;
        break;
    }
}

void Button::EndEdit()
{
    resizeDir = ResizeDir::NONE;
}

void Button::BeginEdit(ResizeDir dir, float mx, float my)
{
    resizeDir = dir;

    startMX = mx;
    startMY = my;

    startX = bData.pos.x;
    startY = bData.pos.y;
    startW = bData.width;
    startH = bData.height;
}

ResizeDir Button::GetResizeDir(float mx, float my) const
{
    float l = bData.pos.x - bData.width / 2;
    float r = bData.pos.x + bData.width / 2;
    float t = bData.pos.y - bData.height / 2;
    float b = bData.pos.y + bData.height / 2;

    auto in = [&](float x1, float y1, float x2, float y2)
        {
            return mx >= x1 && mx <= x2 && my >= y1 && my <= y2;
        };

    if (in(l - HANDLE_SIZE, t - HANDLE_SIZE, l + HANDLE_SIZE, t + HANDLE_SIZE)) return ResizeDir::TOP_LEFT;
    if (in(r - HANDLE_SIZE, t - HANDLE_SIZE, r + HANDLE_SIZE, t + HANDLE_SIZE)) return ResizeDir::TOP_RIGHT;
    if (in(l - HANDLE_SIZE, b - HANDLE_SIZE, l + HANDLE_SIZE, b + HANDLE_SIZE)) return ResizeDir::BOTTOM_LEFT;
    if (in(r - HANDLE_SIZE, b - HANDLE_SIZE, r + HANDLE_SIZE, b + HANDLE_SIZE)) return ResizeDir::BOTTOM_RIGHT;

    if (in(l - HANDLE_SIZE, t, l + HANDLE_SIZE, b)) return ResizeDir::LEFT;
    if (in(r - HANDLE_SIZE, t, r + HANDLE_SIZE, b)) return ResizeDir::RIGHT;
    if (in(l, t - HANDLE_SIZE, r, t + HANDLE_SIZE)) return ResizeDir::TOP;
    if (in(l, b - HANDLE_SIZE, r, b + HANDLE_SIZE)) return ResizeDir::BOTTOM;

    return ResizeDir::NONE;
}
#endif
void Button::RebuildFont()
{
    if (fontHandle != -1)
        DeleteFontToHandle(fontHandle);

    fontHandle = CreateFontToHandle(nullptr, bData.fontSize, -1);
}

