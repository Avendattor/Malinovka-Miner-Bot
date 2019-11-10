#include "stdafx.h"
#include "Cheat.h"
#include "Math.h"

Global g;

Cheat::Cheat()
{
	toAttach(windowName);
	clientBase = dwGetModuleBaseAddress(moduleName);
	std::cout << "Client Base is: " << std::hex << clientBase << std::endl;
	ReadProcessMemory(hProcess, (LPCVOID)(clientBase + 0x79B9BC), &baseAddress, sizeof(baseAddress), NULL);
	std::cout << "Base Addy is: " << std::hex << baseAddress << std::endl;
	
	Sleep(5000);
}
bool Cheat::toAttach(LPCSTR process)
{
	hWnd = FindWindowA(0, process);
	if (hWnd == NULL)
	{
		std::cerr << "Could not find window." << std::endl;
		Sleep(5000);
		exit(-1);
	}
	else
	{
		GetWindowThreadProcessId(hWnd, &pID);
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pID);

		if (pID == NULL)
		{
			std::cerr << "Process is not found" << std::endl;
			Sleep(5000);
			exit(-1);
		}
		else
		{
			std::cout << "Ok!" << std::endl;
			return true;
		}
	}
	return false;
}
DWORD Cheat::dwGetModuleBaseAddress(LPCSTR lpszModuleName)
{
	DWORD dwModuleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
	MODULEENTRY32 ModuleEntry32 = { 0 };
	ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &ModuleEntry32))
	{
		do {
			if (_stricmp(ModuleEntry32.szModule, lpszModuleName) == 0)
			{
				dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
				break;
			}
		} while (Module32Next(hSnapshot, &ModuleEntry32));
	}
	CloseHandle(hSnapshot);
	return dwModuleBaseAddress;
}
void Cheat::render()
{
	DWORD p1;
	ReadProcessMemory(hProcess, (LPCVOID)(baseAddress), &p1, sizeof(p1), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(p1 + 0x14), &p1, sizeof(p1), NULL);

	// Player Position
	ReadProcessMemory(hProcess, (LPCVOID)(p1 + 0x30), &player.posX, sizeof(player.posX), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(p1 + 0x34), &player.posY, sizeof(player.posY), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(p1 + 0x38), &player.posZ, sizeof(player.posZ), NULL);
	
	ReadProcessMemory(hProcess, (LPCVOID)(clientBase + 0x76F248), &player.vCamPosition, sizeof(player.vCamPosition), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(clientBase + 0x76F258), &player.hCamPosition, sizeof(player.hCamPosition), NULL);
	std::cout << "Horizontal Cam Position is: " << std::hex << player.hCamPosition << std::endl;

	ReadProcessMemory(hProcess, (LPCVOID)(clientBase + 0x7729C0), &player.moveSpeed, sizeof(player.moveSpeed), NULL);
}
void Cheat::print()
{
	render();
	std::cout << g._copyright << std::endl;

	std::cout << "Player position: " << std::endl;
	std::cout << "[X]: " << player.posX << std::endl;
	std::cout << "[Y]: " << player.posY << std::endl;
	std::cout << "[z]: " << player.posZ << std::endl;

	std::cout << "Move speed: " << player.moveSpeed << std::endl;
	std::cout << "Camera information: " << std::endl;
	std::cout << "Vertical value: " << player.vCamPosition  << std::endl;
	std::cout << "Horizontal value: " << player.hCamPosition << std::endl << std::endl;
}
Cheat::~Cheat()
{

}
