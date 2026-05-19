#include "stdafx.h"
#include "Game.h"
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{

    ChangeWindowMode(TRUE);
    SetGraphMode(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, 32);
    if (DxLib_Init() == -1) return -1;
    SetDrawScreen(DX_SCREEN_BACK);

    Game game;
    game.Initialize();

    while (game.Run()){
    }

    game.Finalize();

    DxLib_End();
    return 0;
}