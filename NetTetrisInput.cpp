#include "stdafx.h"
#include "NetTetrisInput.h"

void NetTetrisInput::SetState(const NetworkInputState& newState)
{
	state.left = newState.left;
	state.right = newState.right;
	state.softDrop = newState.softDrop;

	state.rotateLeft |= newState.rotateLeft;
	state.rotateRight |= newState.rotateRight;
	state.hold |= newState.hold;
	state.hardDrop |= newState.hardDrop;
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

void NetTetrisInput::ClearTriggers()
{
	state.rotateLeft = false;
	state.rotateRight = false;
	state.hold = false;
	state.hardDrop = false;
}