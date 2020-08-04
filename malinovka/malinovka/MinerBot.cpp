#include <queue>
#include <Windows.h>
#include "MinerBot.h"
#include "include/Math.h"
#include <ctime>
#include <thread>
#include <future>

zPOINT3 origPickPoint; // Random generation
zPOINT3 origPutPoint(1193.69f, 816.69f, 1002.0f);

zPOINT3 firstStoppingPoint(1215.19f, 825.394f, 1002.0f);
zPOINT3 secondStoppingPoint(1216.88f, 816.068f, 1002.0f);

void MinerBot::Run()
{
	setlocale(LC_ALL, "Russian");

	if (origPickPoint.x == 0 && origPickPoint.y == 0 && origPickPoint.z == 0)
	{
		int randomNumber = Cheat::GetRandomNumber(5);

		switch (randomNumber)
		{
			case 5:
			{
				zPOINT3 newPoint(1227.56f, 820.688f, 1002.0f);
				origPickPoint = newPoint;
				break;
			}
			case 4:
			{
				zPOINT3 newPoint(1223.22f, 825.408f, 1002.0f);
				origPickPoint = newPoint;
				break;
			}
			case 3:
			{
				zPOINT3 newPoint(1228.6f, 819.753f, 1002.f);
				origPickPoint = newPoint;
				break;
			}
			case 2:
			{
				zPOINT3 newPoint(1223.67f, 826.013f, 1002.f);
				origPickPoint = newPoint;
				break;
			}
			case 1:
			{
				zPOINT3 newPoint(1223.54f, 824.865f, 1002.f);
				origPickPoint = newPoint;
				break;
			}
		}
	}

	if ((time(0) % 3600) % 5 == 0)
	{
		SendKey(static_cast<UINT>(VK_RETURN), true);
	}


	if (Math::dist(player.GetPosition(), origPickPoint))
	{
		zVEC3 PickToPut(origPickPoint - player.GetPosition());		// Вектор от точки взятия руды до точки сброса
		zVEC3 PlayerToPic(player.GetPosition() - origPickPoint);	// Вектор от игрока до взятия руды
		zVEC3 PlayerToPut(player.GetPosition() - origPutPoint);
		float dist = Math::dist(player.GetPosition(), origPickPoint);

		if (Math::dist(player.GetPosition(), firstStoppingPoint) < 0.1)
		{
			zPOINT3 dodgePoint(1215.47f, 823.51f, 1002.0f);
			SetHorizontalCameraValue(atan2(player.GetPosition().y - dodgePoint.y, player.GetPosition().x - dodgePoint.x));
		}
		else if (Math::dist(player.GetPosition(), secondStoppingPoint) < 0.1)
		{
			zPOINT3 dodgePoint(1216.74f, 817.826f, 1002.0f);
			SetHorizontalCameraValue(atan2(player.GetPosition().y - dodgePoint.y, player.GetPosition().x - dodgePoint.x));
		}
		else
		{
			if (dist > 150)
			{
				CHEAT_CLEAR_CONSOLE;
				std::cout << "[!] Персонаж не находится в шахте." << std::endl;
				std::cout << "Бот отключён." << std::endl;

				SendKey(static_cast<char>('w'), true); // Stop moving
				SendKey(static_cast<UINT>(VK_SPACE), true);
				system("pause");
				exit(-1);
			}

			if (PayDayDropper == true && ((time(0) % 3600) / 60 == 59 && (time(0) % 3600) % 60 > 35))
			{
				SendKey(static_cast<char>('w'), true);
				SendKey(static_cast<UINT>(VK_SPACE), true);

				SendKey(static_cast<UINT>(VK_ESCAPE), true);
				SendKey(static_cast<UINT>(VK_ESCAPE), false);
				Sleep(40000);
				SendKey(static_cast<UINT>(VK_ESCAPE), false);
			}

			if (player.GetMoveSpeed() == 0)
			{
				SendKey(static_cast<char>('w'), false);
				SendKey(static_cast<UINT>(VK_SPACE), false);
			}

			if (Cheat::GetRandomNumber(STOPPING_INTERVAL) == STOPPING_INTERVAL)
			{
				SendKey(static_cast<char>('w'), true); // Stop moving
				SendKey(static_cast<UINT>(VK_SPACE), true);
				Sleep(1000);
			}

			if (status == 0)
			{
				SetHorizontalCameraValue(atan2(PlayerToPic.y, PlayerToPic.x));
				float dist = Math::dist(origPickPoint, player.GetPosition());

				if (dist < 5) // Disable sprint near the dig-point
					SendKey(static_cast<UINT>(VK_SPACE), true);
				if (dist > 9.f && dist < 30.f)
				{
					if (player.GetMoveSpeed() >= 4)
					{
						if (Cheat::GetRandomNumber(JUMP_INTERVAL) == JUMP_INTERVAL)
						{
							SendKey(static_cast<UINT>(VK_SPACE), true);
							keybd_event(VK_LSHIFT, 0, 0, 0);
							Sleep(100);
							keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);
						}
					}
				}
				else if (dist <= MIN_PICK_DISTANCE)
				{
					status = 1;
					SendKey(static_cast<char>('w'), true);

					// Digging timer
					Sleep(10000);
				}
			}
			else if (status == 1)
			{
				// Avoid a collision with a stone
				if (player.GetPosition().y >= 825.f) // чтобы игрок не врезался в камень
				{
					zPOINT3 fakePickPoint(1218.12f, 823.408f, 1002.0f);
					SetHorizontalCameraValue(atan2(player.GetPosition().y - fakePickPoint.y, player.GetPosition().x - fakePickPoint.x));
				}
				else SetHorizontalCameraValue(atan2(PlayerToPut.y, PlayerToPut.x));

				if ((player.GetMoveSpeed() > 4) && Math::dist(origPickPoint, player.GetPosition()) >= 2) status = 0;
				if (Math::dist(origPutPoint, player.GetPosition()) <= 1)
				{
					status = 0;
				}
			}
		}
	}
}

MinerBot::MinerBot() : Cheat()
{
	LoadKeyboardLayoutA("0x0409", KLF_ACTIVATE | KLF_SETFORPROCESS);
	Render();

	std::thread thread_print(&MinerBot::Print, this);
	bool isTimeToHealth = false;

	while (true)
	{
		if (CheatMode == true && ((GetPrevRenderHealth() < player.GetHealth()) || isTimeToHealth == true))
		{
			isTimeToHealth = true;
			if (Math::dist(origPutPoint, player.GetPosition()) <= 1)
			{
				// Queue of WayPoint's
				std::queue<zPOINT3> PointList;

				// WayPoints to the pick-up
				PointList.push(zPOINT3::init(1187.63f, 817.081f, 1002.01f));
				PointList.push(zPOINT3::init(1177.87f, 814.784f, 1002.01f));

				// WayPoints to the exit
				PointList.push(zPOINT3::init(1182.96f, 814.83f, 1002.01f));
				PointList.push(zPOINT3::init(1184.63f, 813.512f, 1002.01f));
				PointList.push(zPOINT3::init(1185.13f, 812.024f, 1002.01f));
				PointList.push(zPOINT3::init(1192.37f, 811.641f, 1002.01f));

				// WayPoints to the pick-up
				PointList.push(zPOINT3::init(2794.55f, 2705.54f, 14.8751f));
				PointList.push(zPOINT3::init(2786.79f, 2681.86f, 14.8751f));
				PointList.push(zPOINT3::init(2794.55f, 2705.54f, 14.8751f));

				// INVERSE STEPS
				PointList.push(zPOINT3::init(2786.79f, 2681.86f, 14.8751f));
				PointList.push(zPOINT3::init(2794.55f, 2705.54f, 14.8751f));

				PointList.push(zPOINT3::init(2796.42f + 1.88f - 0.376f * 2.5, 2712.08f + 2.22f - 0.444f * 2.5, 14.8751f));
				PointList.push(zPOINT3::init(1185.13f, 812.024f, 1002.01f));
				PointList.push(zPOINT3::init(1184.15f, 815.22f, 1002.01f));
				PointList.push(zPOINT3::init(1177.87f, 814.784f, 1002.01f));

				PointList.push(zPOINT3::init(1183.01f, 817.343f, 1002.01f));
				SendKey(static_cast<UINT>(VK_SPACE), true);
				SendKey(static_cast<char>('w'), false);

				while (PointList.empty() == false)
				{
					if (PayDayDropper == true && ((time(0) % 3600) / 60 == 59 && (time(0) % 3600) % 60 > 55))
					{
						SendKey(static_cast<char>('w'), true);
						SendKey(static_cast<UINT>(VK_SPACE), true);
						SendKey(static_cast<UINT>(VK_ESCAPE), true);
						SendKey(static_cast<UINT>(VK_ESCAPE), false);

						Sleep(10000);
						SendKey(static_cast<UINT>(VK_ESCAPE), false);
						
						Sleep(1000);			
						SendKey(static_cast<char>('w'), false);
					}
					SetHorizontalCameraValue(atan2(player.GetPosition().y - PointList.front().y, player.GetPosition().x - PointList.front().x));

					Sleep(50);
					SendKey(static_cast<UINT>(VK_RETURN), true);
					if (Math::dist(player.GetPosition(), PointList.front()) < 0.9)
					{
	
					//	std::cout << PointList.size() << std::endl;
						PointList.pop();


						//if (PointList.size() == 16 || PointList.size() == 15 || PointList.size() == 9 || PointList.size() == 7 || PointList.size() == 2) // Ставим таймеры чтобы было время на кнопку ENTER

					}

					if (PointList.size() == 1 && player.GetMoveSpeed() == 0)
					{
						if ((time(0) % 3600) % 5 == 0)
						{
							SendKey(static_cast<UINT>(VK_RETURN), true);
							Sleep(1000);
						}
					}
					Render();
				}

				isTimeToHealth = false;
				SendKey(static_cast<UINT>(VK_RETURN), true); 
				player.GetHealth() = GetPrevRenderHealth();
			}
			else Run();
		}
		else Run();

		Render();
		//DebugLog();

		CHEAT_REFRESH_FREQUANCY;
		//CHEAT_CLEAR_CONSOLE;
	}

}
MinerBot::~MinerBot()
{
}
