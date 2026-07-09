#pragma once

#include "stdafx.h"
#include "Job.h"
#include "DxLib.h"   // ← マウス操作で必要

class InputManager : public Job {
private:
    // キーボード
    int keys[256];

    // マウスボタン
    int mouseButtons[3];     // 0:左, 1:右, 2:中
	int oldMouseButtons[3];  // 前フレームの状態を保存する配列

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
    bool IsPress(int key) const { return keys[key] > 0; }
    bool IsTrigger(int key) const { return keys[key] == 1; }
    bool IsRepeat(int key, int das, int arr) const;

    //----------------------------
    //  マウス
    //----------------------------
    // 位置
    int GetMouseX() const { return mouseX; }
    int GetMouseY() const { return mouseY; }
	Vec2i GetMousePosition() const { return Vec2i{ mouseX, mouseY }; }
    Vec2f GetMousePositionF() const { return Vec2f{ MyStd::Cast<float>(mouseX), MyStd::Cast<float>(mouseY) }; }

    // ボタン Press
    bool IsMousePressLeft()   const { return mouseButtons[0]; }
    bool IsMousePressRight()  const { return mouseButtons[1]; }
    bool IsMousePressMiddle() const { return mouseButtons[2]; }

    // ボタン Trigger
    bool IsMouseTriggerLeft()   const { return mouseButtons[0] && !oldMouseButtons[0]; }
    bool IsMouseTriggerRight()  const { return mouseButtons[1] && !oldMouseButtons[1]; }
    bool IsMouseTriggerMiddle() const { return mouseButtons[2] && !oldMouseButtons[2]; }

	// ボタン Release
	bool IsMouseReleaseLeft()   const { return !mouseButtons[0] && oldMouseButtons[0]; }
	bool IsMouseReleaseRight()  const { return !mouseButtons[1] && oldMouseButtons[1]; }
	bool IsMouseReleaseMiddle() const { return !mouseButtons[2] && oldMouseButtons[2]; }

    // ホイール
    int GetWheelRot() const { return wheelRot; }

    //----------------------------
    // シングルトン取得
    //----------------------------
    static InputManager& GetInstance() {
        static InputManager instance;
        return instance;
    }
};
