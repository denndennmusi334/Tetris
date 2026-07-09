#pragma once
#include "SceneManager.h"

class Game
{
private:
    SceneManager* sceneMgr = nullptr;
public:
    void Initialize();
    void InitManager();
    void Finalize();
    void Draw();
    void Update();
    void Kill();

    bool Run();
};
