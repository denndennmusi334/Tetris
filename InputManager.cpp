#include "stdafx.h"
#include "InputManager.h"

void InputManager :: Update() 
{
    // 旧キー保存
    memcpy(oldMouseButtons, mouseButtons, sizeof(mouseButtons));

    // キーボード更新
    GetHitKeyStateAllEx(keys);

    // マウス位置
    GetMousePoint(&mouseX, &mouseY);

    // マウスボタン
    mouseButtons[0] = (int)GetMouseInput() & MOUSE_INPUT_LEFT;
    mouseButtons[1] = (int)GetMouseInput() & MOUSE_INPUT_RIGHT;
    mouseButtons[2] = (int)GetMouseInput() & MOUSE_INPUT_MIDDLE;

    // ホイール
    wheelRot = GetMouseWheelRotVol();
}
/// <summary>
/// 長押ししていると進むやつの判定関数.
/// 引数は時間ではなくフレーム数で指定する.
/// </summary>
/// <param name="key">Dxlibのキーコード</param>
/// <param name="das">長押し開始までの待期フレーム</param>
/// <param name="arr">移動間隔フレーム</param>
/// <returns></returns>
bool InputManager::IsRepeat(
    int key,
    int das,
    int arr) const
{
    int frame = keys[key];

    if (frame == 1)
        return true;

    if (frame > das)
    {
        return ((frame - das) % arr) == 0;
    }

    return false;
}
