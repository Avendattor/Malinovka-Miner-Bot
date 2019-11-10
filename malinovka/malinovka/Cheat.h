#pragma once
#include "stdafx.h"
#include "malinovka.h"
#include "Player.h"


class Cheat
{
private:
	char moduleName[19] = "malinovka_core.dll";
	char windowName[34] = "Malinovka Client (SA-MP 0.3.7-R3)";

public:
	Player player;

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

	void sendKey(char key);
	void sendKey(UINT ch);
	void debugLog();

	inline void setVerticalCameraValue(float v) { WriteProcessMemory(hProcess, (LPVOID)(clientBase + 0x76F248), &v, sizeof(v), NULL); };
	inline void setHorizontalCameraValue(float v) { WriteProcessMemory(hProcess, (LPVOID)(clientBase + 0x76F258), &v, sizeof(v), NULL); };

	~Cheat();
}; 

extern Global g;