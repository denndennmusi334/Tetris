#pragma once
#include "GameObject.h"
class TextInputBox : public GameObject
{
private:
	std::string text = "\0";

	bool isUse = false;

	bool isActive = false;

	float width;
	float height;

	unsigned int selectColor = 0;
	unsigned int idleColor = 0;
public:
	TextInputBox(const Vec2f& pos, float width, float height)
		:width(width), height(height) {
		SetPosition(pos);
		selectColor = GetColor(255, 255, 0);
		idleColor = GetColor(255, 255, 255);
	}
	void Update() override;
	void Draw(const Camera& camera) override;

	void SetActive(bool active) { isActive = active; }

	std::string GetText() const { return text; }

	bool IsActiveCheck() const;
};

