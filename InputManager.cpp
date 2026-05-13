#include "stdafx.h"
#include "InputManager.h"

void InputManager :: Update() 
{
    // 旧キー保存
    memcpy(oldKeys, keys, sizeof(keys));
    memcpy(oldMouseButtons, mouseButtons, sizeof(mouseButtons));

    // キーボード更新
    GetHitKeyStateAll(keys);

    // マウス位置
    GetMousePoint(&mouseX, &mouseY);

    // マウスボタン
    mouseButtons[0] = (int)GetMouseInput() & MOUSE_INPUT_LEFT;
    mouseButtons[1] = (int)GetMouseInput() & MOUSE_INPUT_RIGHT;
    mouseButtons[2] = (int)GetMouseInput() & MOUSE_INPUT_MIDDLE;

    // ホイール
    wheelRot = GetMouseWheelRotVol();
}