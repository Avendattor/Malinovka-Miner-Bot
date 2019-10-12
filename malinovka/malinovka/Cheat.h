#pragma once
#include "Player.h"

#define CPed								0xB6F5F0
#define MIDDLE_VERTICAL_CAMERA_POSITION		-0.326023f
# define M_PI								3.14159265358979323846

class Cheat
{
private:
	char moduleName[19] = "malinovka_core.dll";

public:
	Player player;
	bool CheatStatus = false;

	HWND hWnd;
	char * process;
	HANDLE hProcess;
	DWORD baseAddress;
	DWORD pID;

	DWORD clientBase;

	Cheat();
	bool toAttach(LPCSTR process);
	DWORD dwGetModuleBaseAddress(LPCSTR lpszModuleName);
	void render();
	void print();

	void setVerticalCameraValue(float v) 
	{
		WriteProcessMemory(hProcess, (LPVOID)(clientBase + 0x76F248), &v, sizeof(v), NULL);
	}

	inline void setHorizontalCameraValue(float v)
	{
		WriteProcessMemory(hProcess, (LPVOID)(clientBase + 0x76F258), &v, sizeof(v), NULL);
	}

	~Cheat();
}; 

