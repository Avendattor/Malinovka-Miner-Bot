#include "stdafx.h"
#include <vector>
#include <Windows.h>
#include "MinerBot.h"
#include "Math.h"

void MinerBot::run()
{
		if (Math::dist(player.posX, player.posY, player.posZ, X, Y, Z))
		{
			std::vector<float> v1(3);
			std::vector<float> v2(3);
			std::vector<float> v3(3);

			v1[0] = X - player.posX;
			v1[1] = Y - player.posY;
			v1[2] = Z - player.posZ;

			v2[0] = 1195.4f - X;
			v2[1] = 816.69f - Y;
			v2[2] = 1002.f - Z;

			v3[0] = player.posX - 1193.07f;
			v3[1] = player.posY - 816.567f;
			v3[2] = player.posZ - 1002.f;

			float dist = Math::dist(X, Y, Z, player.posX, player.posY, player.posZ);

			if (dist > 150)
			{
				system("cls");
				std::cout << "BOT: You are not in the mine." << std::endl;
				std::cout << "BOT: The bot is disabled." << std::endl;

				sendKey(static_cast<char>('w'), true); // stop moving
				sendKey(static_cast<UINT>(VK_SPACE), true);
				system("pause");
				exit(-1);
			}

			if (player.posZ > 1003.5f)
			{
				system("cls");
				std::cout << "BOT: Admin slapped you" << std::endl;
				std::cout << "BOT: The bot is disabled." << std::endl;

				sendKey(static_cast<char>('w'), true); // stop moving
				sendKey(static_cast<UINT>(VK_SPACE), true);
				system("pause");
				exit(-1);
			}

			if (player.moveSpeed == 0)
			{
				sendKey(static_cast<char>('w'), false);
 				sendKey(static_cast<UINT>(VK_SPACE), false);
			}

			if (status == 0)
			{
				setHorizontalCameraValue(atan2(v1[1], v1[0]) + 3.f); // to the point
				
				float dist = Math::dist(X, Y, Z, player.posX, player.posY, player.posZ);
				if (dist > 9.f && dist < 30.f)
				{
					if (player.moveSpeed >= 4)
					{
						sendKey(static_cast<UINT>(VK_SPACE), true);
						keybd_event(VK_LSHIFT, 0, 0, 0);
						Sleep(50);
						keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);
					}
				}
				else if (dist <= 1.5)
				{
					status = 1;
					sendKey(static_cast<char>('w'), true);
					Sleep(10000);
				}
			}
			else if (status == 1)
			{
				setHorizontalCameraValue(atan2(v3[1], v3[0]));
				if ((player.moveSpeed > 4) && Math::dist(X, Y, Z, player.posX, player.posY, player.posZ) >= 2) status = 0;
				if (Math::dist(1193.07f, 816.567f, 1002.f, player.posX, player.posY, player.posZ) <= 1.5)
				{
					status = 0;
				}
			}
		}
}

MinerBot::MinerBot() : Cheat()
{
	LoadKeyboardLayoutA("0x0409", KLF_ACTIVATE | KLF_SETFORPROCESS);
	print();

	while (true)
	{
		render();
		run();
		Sleep(50);
		//system("cls");
	}
}

MinerBot::~MinerBot()
{
}
