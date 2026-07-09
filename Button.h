#pragma once
#include "GameObject.h"

enum class BUTTON_TYPE
{
    SQUARE,
    ELLIPSE,
};

enum class ResizeDir
{
    NONE,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
};

struct ButtonData
{
    std::basic_string<TCHAR> text = _T("");
    int fontSize = 24;

	Vec2f pos = { 0.0f, 0.0f };
    float width = 20.0f;
    float height = 20.0f;

	int id = 0;

    BUTTON_TYPE button_type = BUTTON_TYPE::SQUARE;

    unsigned int neutral_color = 0xffffffff;
    unsigned int on_color = 0xffcccccc;
    unsigned int text_color = 0xffffffff;

    BOOL fill_flag = FALSE;
    int grH = -1;
};

class Button : public GameObject
{
private:
    ButtonData bData;
    int fontHandle = -1;

    bool isPush = false;
    bool isOn = true;

	bool isActive = true;
#if USE_BUTTON_EDIT
    // ===== Editor óp =====
    ResizeDir resizeDir = ResizeDir::NONE;

    float startMX = 0.0f;
    float startMY = 0.0f;

    float startX = 0.0f;
    float startY = 0.0f;
    float startW = 0.0f;
    float startH = 0.0f;
#endif
public:
    Button() = default;
    Button(const ButtonData& data);

    void Initialize() override;
    void Finalize() override;
    void Draw(const Camera& camera) override ;

	void SetActive(bool active) { isActive = active; }
	bool IsActive() const { return isActive; }

    // îªíË
    BOOL HitCheck(float mx, float my) const;

#if USE_BUTTON_EDIT
    // Editor ëÄçÏ
    ResizeDir GetResizeDir(float mx, float my) const;
    void BeginEdit(ResizeDir dir, float mx, float my);
    void Update(float mx, float my);
    void EndEdit();
#else
    void Update() override;
#endif
    bool GetIsPush() { return isPush; }
    ButtonData& GetData(){ return bData; }

    void RebuildFont();
};
