#pragma once
#include "include/Math.h"
#define player Player::GetInstance()

class Player // Singleton
{
private:
	zPOINT3 pos;
	int moveSpeed;
	float hCamPosition;
	float vCamPosition;
	float health;

public:

	inline float& GetHealth() { return health; }
	inline zPOINT3& GetPosition() { return pos; }
	inline int& GetMoveSpeed() { return moveSpeed; }
	inline float& GetHorizontalCameraPosition() { return hCamPosition; }
	inline float& GetVerticalCameraPosition() { return vCamPosition; }
	inline zPOINT3& SetPosition() { return pos; }

	static Player& GetInstance()
	{
		static Player npc;

		return npc;
	}

	Player();
	~Player();
};