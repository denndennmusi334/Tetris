#pragma once
#include "Job.h"
class GameMap :
    public Job
{
private:

public:
	GameMap();
	~GameMap();
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};

