#pragma once
#include "Cheat.h"
#include "include/Math.h"

#define Bot MinerBot::GetInstance()

class MinerBot : public Cheat
{
private:
	int status = 0;

	const BYTE JUMP_INTERVAL = 3;
	const BYTE STOPPING_INTERVAL = 50;
	const float MIN_PICK_DISTANCE = 1.0f;

public:
	static bool MinerMode;

	void Run();
	static MinerBot& GetInstance()
	{
		static MinerBot bot;

		return bot;
	}

	MinerBot();
	~MinerBot();
};
