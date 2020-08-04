#pragma once
#include "malinovka.h"
#include "Player.h"
#include "Event.h"
#include <vector>

#define CHEAT_REFRESH_FREQUANCY Sleep(20)
#define CHEAT_CLEAR_CONSOLE system("cls");

class Cheat
{
private:
	std::string moduleName = "malinovka_game.exe";
	std::string windowName;
public:
	// Системная информация о процессе
	PROCESSENTRY32	entry;
	HWND			hWnd;
	char*			process;
	HANDLE			hProcess;
	DWORD			baseAddress;
	DWORD			pID;
	DWORD			clientBase;

	Cheat();
	bool ToAttach();
	DWORD dwGetModuleBaseAddress(LPCSTR lpszModuleName);
	std::vector<std::string> getAllModules();

	void Render();
	void Print();

	// Console
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Методы для взаимодействия с клавиатурой
	void SendKey(char ch, bool withKeyUp);
	void SendKey(UINT ch, bool withKeyUp);

	// Game events
	bool CheatMode = false;
	bool PayDayDropper = false;

	// Отладочная информация
	void DebugLog();

	inline void SetVerticalCameraValue(float v) { WriteProcessMemory(hProcess, (LPVOID)(clientBase + 0x76F248), &v, sizeof(v), NULL); };
	inline void SetHorizontalCameraValue(float v) { WriteProcessMemory(hProcess, (LPVOID)(clientBase + 0x76F258), &v, sizeof(v), NULL); };
	inline void SetPlayerHealth(float v)
	{
		DWORD p1;
		ReadProcessMemory(hProcess, (LPCVOID)(baseAddress), &p1, sizeof(p1), NULL);
		WriteProcessMemory(hProcess, (LPVOID)(p1 + 0x540), &v, sizeof(v), NULL);
	}
	static int GetRandomNumber(int rMax);

	float GetPrevRenderHealth();

	~Cheat();
};

extern Global g;