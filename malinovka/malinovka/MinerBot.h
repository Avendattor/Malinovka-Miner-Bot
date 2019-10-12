#pragma once
#include "Cheat.h"
class MinerBot : public Cheat
{
private:
		float X = 1223.22f, Y = 825.408f, Z = 1002.0f; // Êîîðäèíàòû
		int status = 0;
public:
	void run();
	MinerBot();
	~MinerBot();
};

