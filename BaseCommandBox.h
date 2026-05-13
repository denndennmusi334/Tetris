#pragma once
#include "Job.h"

enum class CommandType
{
    None,

    Attack,

    OpenMagic, // 「まほう」を開く
    Magic,     // 魔法を選択した

    Defend,
    Escape,
    Back
};

struct CommandResult
{
    CommandType type = CommandType::None;
    int index = -1; 
};

class BaseCommandBox : public Job
{
protected:
    CommandResult result;
    bool isDecided = false;

public:
	BaseCommandBox();
    virtual ~BaseCommandBox();

    bool IsDecided() const { return isDecided; }
    CommandResult GetResult() const { return result; }

    void ResetDecision()
    {
        isDecided = false;
        result = {};
    }
};

