#include "stdafx.h"
#include "NetTetrisInput.h"

void NetTetrisInput::SetState(const NetworkInputState& s)
{
	state = s;
}

void NetTetrisInput::Update()
{
	UpdateAction(
		TetrisAction::MoveLeft,
		state.left);
	UpdateAction(
		TetrisAction::MoveRight,
		state.right);
	UpdateAction(
		TetrisAction::SoftDrop,
		state.softDrop);
	UpdateAction(
		TetrisAction::HardDrop,
		state.hardDrop);
	UpdateAction(
		TetrisAction::RotateLeft,
		state.rotateLeft);
	UpdateAction(
		TetrisAction::RotateRight,
		state.rotateRight);
	UpdateAction(
		TetrisAction::Hold,
		state.hold);
}