#pragma once

enum class TetrisAction
{
	MoveLeft,
	MoveRight,

	SoftDrop,
	HardDrop,

	RotateLeft,
	RotateRight,

	Hold,

	Count
};

struct ActionState
{
	bool press = false;
	bool trigger = false;
	bool release = false;

	int holdFrame = 0;
};

struct KeyBinding
{
	int moveLeft;
	int moveRight;

	int softDrop;
	int hardDrop;

	int rotateLeft;
	int rotateRight;

	int hold;
};

class TetrisInput
{
private:

	ActionState actions[(int)TetrisAction::Count];

	KeyBinding keyBinding = {
		KEY_INPUT_A, // MoveLeft
		KEY_INPUT_D, // MoveRight
		KEY_INPUT_S, // SoftDrop
		KEY_INPUT_SPACE, // HardDrop
		KEY_INPUT_Q, // RotateLeft
		KEY_INPUT_E, // RotateRight
		KEY_INPUT_LSHIFT // Hold
	};

public:

	TetrisInput(){}
	TetrisInput(KeyBinding& p) : keyBinding(p) {}

	virtual void Update();

	void UpdateAction(TetrisAction action, bool nowPress);

	bool IsPress(TetrisAction action) const {
		return actions[(int)action].press;
	}

	bool IsTrigger(TetrisAction action) const {
		return actions[(int)action].trigger;
	}

	bool IsRepeat(TetrisAction action, int das, int arr) const {
		const ActionState& a = actions[(int)action];
		if (a.trigger)
		{
			return true;
		}
		if (a.press && a.holdFrame >= das)
		{
			return (a.holdFrame - das) % arr == 0;
		}
		return false;
	}
};

