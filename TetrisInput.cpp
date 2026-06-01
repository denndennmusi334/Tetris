#include "stdafx.h"
#include "TetrisInput.h"
#include "InputManager.h"

void TetrisInput::Update()
{
    auto& input =
        InputManager::GetInstance();

    UpdateAction(
        TetrisAction::MoveLeft,
        input.IsPress(keyBinding.moveLeft));

    UpdateAction(
        TetrisAction::MoveRight,
        input.IsPress(keyBinding.moveRight));

    UpdateAction(
        TetrisAction::SoftDrop,
        input.IsPress(keyBinding.softDrop));

    UpdateAction(
        TetrisAction::HardDrop,
        input.IsPress(keyBinding.hardDrop));

    UpdateAction(
        TetrisAction::RotateLeft,
        input.IsPress(keyBinding.rotateLeft));

    UpdateAction(
        TetrisAction::RotateRight,
        input.IsPress(keyBinding.rotateRight));

    UpdateAction(
        TetrisAction::Hold,
        input.IsPress(keyBinding.hold));
}

void TetrisInput::UpdateAction(
    TetrisAction action,
    bool nowPress)
{
    auto& a =
        actions[(int)action];

    bool old =
        a.press;

    a.press =
        nowPress;

    a.trigger =
        nowPress && !old;

    a.release =
        !nowPress && old;

    if (a.press)
    {
        a.holdFrame++;
    }
    else
    {
        a.holdFrame = 0;
    }
}