#pragma once
#include "TetrisInput.h"

struct NetworkInputState
{
    bool left;
    bool right;

    bool softDrop;
    bool hardDrop;

    bool rotateLeft;
    bool rotateRight;

    bool hold;
};

class NetTetrisInput :
    public TetrisInput
{
private:

    NetworkInputState state;

public:

    NetTetrisInput() :
        state{ false, false, false, false, false, false, false }{ }
	
	

    void SetState(
        const NetworkInputState& s);

    void Update() override;
};

