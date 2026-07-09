#include "stdafx.h"
#include "DrawManager.h"

void DrawManager::Add(Drawable* obj)
{
    if (std::find(drawList.begin(), drawList.end(), obj) == drawList.end())
    {
        drawList.push_back(obj);
    }
    obj->onLayerChanged = [this]()
        {
            isSorted = false;
        };

    isSorted = false; // 新しいオブジェクトが追加されたので、ソートが必要になる
}

void DrawManager::DrawAll(const Camera& camera)
{
    if (!isSorted)
    {
        std::sort(drawList.begin(), drawList.end(),
            [](Drawable* a, Drawable* b)
            {
                return a->GetDrawLayer() < b->GetDrawLayer();
            });
        isSorted = true; // ソートが完了したので、フラグをリセット.
    }

    // 描画
    for (auto* obj : drawList)
    {
        if (obj)
            obj->Draw(camera);
        else
            MessageBoxW(NULL, L"DrawObjがうまく消せていない可能性があります。", L"警告", MB_OK);  //バグ対策.
    }
}

void DrawManager::Clear()
{
	drawList.clear();
	isSorted = true;
}
