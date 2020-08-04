#pragma once
class Event
{
public:
	static void OnPlayerRun(int moveSpeed);
	static void OnPlayerStand();
	static void OnPlayerPickup(int pickupID);
};