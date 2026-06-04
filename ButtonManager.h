#pragma once
#include "Button.h"

class ButtonManager
{
private:
    std::vector<Button*> buttons;

    Button* selected = nullptr;
    ResizeDir activeDir = ResizeDir::NONE;

public:
    Button& AddButton(const ButtonData& data);

    ButtonData& GetButtonData(int num)
    {
        for (auto& b : buttons)
        {
			auto& d = b->GetData();
            if (d.id == num) {
				return d;
            }
        }
        return buttons[num]->GetData(); // ← ポインタ経由で呼ぶ
    }

    void SetActive(int num, bool active) {
        for (auto& b : buttons)
        {
            auto& d = b->GetData();
            if (d.id == num) {
                b->SetActive(active);
                return;
            }
        }
    }

    void SetAllActive(bool active)
    {
        for (auto& b : buttons)
        {
            b->SetActive(active);
        }
    }

    void Destroy() {
        for (auto& b : buttons)
        {
            if (b)
                b->Destroy();
        }
		buttons.clear();
    }
    void Update();    // プレイ用ボタン操作
    void Draw();
	void Kill();

    void LoadCSV(const TCHAR* path);

    bool IsRelease(int id) const;  // id指定で押されているか

#if USE_BUTTON_EDIT
    void SaveCSV(const TCHAR* path);
#endif
};
