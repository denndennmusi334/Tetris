#pragma once

#include "stdafx.h"
#include "Job.h"
#include "DxLib.h"   // ← マウス操作で必要

class InputManager : public Job {
private:
    // キーボード
    char keys[256];
    char oldKeys[256];

    // マウスボタン
    int mouseButtons[3];     // 0:左, 1:右, 2:中
    int oldMouseButtons[3];

    // マウス座標
    int mouseX;
    int mouseY;

    // Wheel
    int wheelRot;

    // シングルトン
    InputManager()
        : mouseX(0), mouseY(0), wheelRot(0)
    {
        memset(keys, 0, sizeof(keys));
        memset(oldKeys, 0, sizeof(oldKeys));
        memset(mouseButtons, 0, sizeof(mouseButtons));
        memset(oldMouseButtons, 0, sizeof(oldMouseButtons));
    }
    ~InputManager() = default;

public:
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    void Initialize() override {};
    void Finalize() override {};
    void Update() override;
    void Draw() override {};

    //----------------------------
    //  キーボード
    //----------------------------
    BOOL isPress(int key) const { return keys[key]; }
    BOOL isTrigger(int key) const { return keys[key] && !oldKeys[key]; }

    //----------------------------
    //  マウス
    //----------------------------
    // 位置
    int getMouseX() const { return mouseX; }
    int getMouseY() const { return mouseY; }
	Vec2i GetMousePosition() const { return Vec2i{ mouseX, mouseY }; }
    Vec2f GetMousePositionF() const { return Vec2f{ MyStd::Cast<float>(mouseX), MyStd::Cast<float>(mouseY) }; }

    // ボタン Press
    bool isMousePressLeft()   const { return mouseButtons[0]; }
    bool isMousePressRight()  const { return mouseButtons[1]; }
    bool isMousePressMiddle() const { return mouseButtons[2]; }

    // ボタン Trigger
    bool isMouseTriggerLeft()   const { return mouseButtons[0] && !oldMouseButtons[0]; }
    bool isMouseTriggerRight()  const { return mouseButtons[1] && !oldMouseButtons[1]; }
    bool isMouseTriggerMiddle() const { return mouseButtons[2] && !oldMouseButtons[2]; }

    // ホイール
    int getWheelRot() const { return wheelRot; }

    //----------------------------
    // シングルトン取得
    //----------------------------
    static InputManager& GetInstance() {
        static InputManager instance;
        return instance;
    }
};
