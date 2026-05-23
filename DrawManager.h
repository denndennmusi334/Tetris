#pragma once
#include "Drawable.h"

class DrawManager
{
public:
    void Add(Drawable* obj);
    void DrawAll(const Camera& camera);
    void Clear();

	void DestroyedDrawableCheck()
	{
		drawList.erase(std::remove_if(drawList.begin(), drawList.end(),
			[](Drawable* obj) {return !obj || obj->IsDestroyed();}),
			drawList.end());
	}
private:
    bool isSorted = true; // 描画リストがソートされているかどうかのフラグ
    std::vector<Drawable*> drawList;
};
