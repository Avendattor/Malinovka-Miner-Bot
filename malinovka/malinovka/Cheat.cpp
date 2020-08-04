#include "Cheat.h"
#include "Math.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <clocale>

HWND g_HWND = NULL;
std::string tWindowName;

static BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam) {
	int length = GetWindowTextLength(hWnd);
	char* buffer = new char[length + 1];
	GetWindowText(hWnd, buffer, length + 1);
	std::string windowTitle(buffer);

	// List visible windows with a non-empty title
	if (IsWindowVisible(hWnd) && length != 0) {
		if (windowTitle.substr(0, 16).compare("Malinovka Client") == 0)
		{
			if (tWindowName.size() == 0)
			{
				tWindowName = windowTitle;
				g_HWND = hWnd;
			}
		}
	}
	return TRUE;
}

Global g;
INPUT key;

void Cheat::SendKey(UINT ch, bool withKeyUp = false)
{
	memset(&key, 0, sizeof(INPUT));//Zero the structure.
	key.type = INPUT_KEYBOARD;
	key.ki.dwExtraInfo = GetMessageExtraInfo();//<-- you will need to pinvoke this too.
	key.ki.wScan =
		static_cast<WORD>(MapVirtualKeyEx(VkKeyScanA(ch), MAPVK_VK_TO_VSC, GetKeyboardLayout(0)));//more pinvoking
	key.ki.dwFlags = KEYEVENTF_SCANCODE;//<-- you will probably have to declare this constant somewhere-
										//in your C# program.
										//Ready to send the key-down event.
	SendInput(1, &key, sizeof(INPUT));

	if (withKeyUp == true)
	{
		key.ki.dwExtraInfo = GetMessageExtraInfo();
		key.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;//Key-up need be defined too, or just use the value.
		SendInput(1, &key, sizeof(INPUT));
	}
	//Sleep(1200);//Wait one second before sending key-up.
	//Sending key-up.
	;
}

void Cheat::SendKey(char ch, bool withKeyUp = false)
{
	memset(&key, 0, sizeof(INPUT));//Zero the structure.
	key.type = INPUT_KEYBOARD;
	key.ki.dwExtraInfo = GetMessageExtraInfo();//<-- you will need to pinvoke this too.
	key.ki.wScan =
		static_cast<WORD>(MapVirtualKeyEx(VkKeyScanA(ch), MAPVK_VK_TO_VSC, GetKeyboardLayout(0)));//more pinvoking
	key.ki.dwFlags = KEYEVENTF_SCANCODE;//<-- you will probably have to declare this constant somewhere-
										//in your C# program.
										//Ready to send the key-down event.
	SendInput(1, &key, sizeof(INPUT));

	if (withKeyUp == true)
	{
		key.ki.dwExtraInfo = GetMessageExtraInfo();
		key.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;//Key-up need be defined too, or just use the value.
		SendInput(1, &key, sizeof(INPUT));
	}

	//Sleep(1200);//Wait one second before sending key-up.
				//Sending key-up.
	//key.ki.dwExtraInfo = GetMessageExtraInfo();
	//key.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;//Key-up need be defined too, or just use the value.
	//SendInput(1, &key, sizeof(INPUT));
}

void Cheat::DebugLog()
{
	std::cout << "Health points: " << player.GetHealth() << std::endl;
	std::cout << "[X]: " << player.GetPosition().x << std::endl;
	std::cout << "[Y]: " << player.GetPosition().y << std::endl;
	std::cout << "[Z]: " << player.GetPosition().z << std::endl;

	std::cout << "Move speed: " << player.GetMoveSpeed() << std::endl;
	std::cout << "Camera information: " << std::endl;
	std::cout << "Vertical value: " << player.GetVerticalCameraPosition() << std::endl;
	std::cout << "Horizontal value: " << player.GetHorizontalCameraPosition() << std::endl << std::endl;
	std::cout << "Time: " << (time(0) % 3600) / 60 << ":" << (time(0) % 3600) % 60 << std::endl;
}

bool Cheat::ToAttach()
{
	setlocale(LC_ALL, "Russian");

	EnumWindows(enumWindowCallback, NULL);
	hWnd = g_HWND;
	//hWnd = FindWindow(_T(windowName.c_str()), NULL);
	if (hWnd == NULL)
	{
		system("cls");

		std::cerr << "Бот: Не могу найти окно игры.." << std::endl;
		std::cerr << "Бот: Попробуйте запустить игру от имени администратора." << std::endl;
		Sleep(5000);
		exit(-1);
	}
	else
	{
		GetWindowThreadProcessId(hWnd, &pID);
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pID);

		if (pID == NULL)
		{
			system("cls");
			std::cerr << "Бот: Не могу найти окно игры.." << std::endl;
			std::cerr << "Бот: Попробуйте запустить игру от имени администратора." << std::endl;
			Sleep(5000);
			exit(-1);
		}
		else
		{
			return true;
		}
	}
	return false;
}

Cheat::Cheat()
{
	ToAttach();
	clientBase = dwGetModuleBaseAddress(moduleName.c_str());

	if (clientBase == 0)
	{
		std::vector<std::string> vModules = getAllModules();

		int select = -1;
		do
		{
			system("cls");
			std::cout << "Программа не может найти модуль " << moduleName << std::endl;
			std::cout << "Возможно разработчики сервера изменили наименование файлов." << std::endl;
			std::cout << "Из предложенного списка выберите тот, который кажется вам подозрительным (похож на gta_sa, malinovka, samp)" << std::endl;

			for (int i = 0; i < vModules.size(); i++)
			{
				std::cout << "[" << i + 1 << "] " << vModules[i] << std::endl;
			}
			std::cout << "Выберите номер модуля: "; std::cin >> select;

			if (select < 1 && select > vModules.size() + 1)
			{
				std::cout << "Введён неверный номер модуля" << std::endl;
				system("pause");
				continue;
			}

			clientBase = dwGetModuleBaseAddress(vModules[select - 1].c_str());
			ReadProcessMemory(hProcess, (LPCVOID)(clientBase + 0x79B9BC), &baseAddress, sizeof(baseAddress), NULL);

			if (baseAddress != 0)
			{
				std::cout << "Модуль подключен!" << std::endl;
				system("pause");
				system("cls");
				break;
			}
			else
			{
				std::cout << "К сожалению выбран неверный модуль. Попробуйте выбрать другой." << std::endl;
				system("pause");
			}

		} while (true);

	}

	bool mode = 0;
	setlocale(LC_ALL, "Russian");
	system("cls");

	std::cout << ".::Режимы::." << std::endl;
	std::cout << "[1]: Бесконечный фарм (подходит для игрока первого уровня)" << std::endl;
	std::cout << "(Описание): В момент голода бот сам будет брать еду и возвращаться к работе. Можно не беспокоиться про смерть во время работы на шахте.\nЕсли дополнительно включить функцию пропуск PayDay-я, то это даст возможность постоянно пополнять сытость." << std::endl;
	std::cout << "\n[0]: Обычный режим (подходит для персонажа 2+ уровня)" << std::endl;
	std::cout << "(Описание): Стандартный функционал. Функция пропуска PayDay-я здесь отключена, потому что игрок не может пополнять сытость с помощью пикапа около шахты (уровень больше 1)" << std::endl;
	std::cout << "Введите номер пункта: "; std::cin >> CheatMode;

	system("cls");
	if (CheatMode == 1)
	{
		std::cout << "[Событие]: Вы хотите пропускать PayDay?" << std::endl;
		std::cout << "Да - [1], Нет - [0] " << std::endl;
		std::cout << "Введите номер пункта: "; std::cin >> PayDayDropper;
		system("cls");
		if (PayDayDropper == true)
		{
			std::cout << "[!] Пропуск PayDay активирован" << std::endl;
		}
	}
	else std::cout << "Режим (Обычный режим) - активирован" << std::endl;


	std::cout << "\n[!] У вас есть ровно 5 секунд чтобы захватить окно игры.";
	//std::cout << "Client Base is: " << std::hex << clientBase << std::endl;
	ReadProcessMemory(hProcess, (LPCVOID)(clientBase + 0x79B9BC), &baseAddress, sizeof(baseAddress), NULL);
	//std::cout << "Base Addy is: " << std::hex << baseAddress << std::endl;

	if ((baseAddress) == 0)
	{
		CHEAT_CLEAR_CONSOLE;
		std::cout << "Бот: Не могу найти адрес расположения игры." << std::endl;
		std::cout << "Бот: Пожалуйста, перезайдите в игру." << std::endl;
		Sleep(5000);
		exit(-1);
	}
	Sleep(5000);
	CHEAT_CLEAR_CONSOLE;
}




std::vector<std::string> Cheat::getAllModules()
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
	MODULEENTRY32 ModuleEntry32 = { 0 };
	ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	std::vector<std::string> v;
	int counter = 1;
	if (Module32First(hSnapshot, &ModuleEntry32))
	{
		do
		{
			v.push_back(ModuleEntry32.szModule);
			//std::cout << "[" << counter << "]: " << ModuleEntry32.szModule << std::endl;

		} while (Module32Next(hSnapshot, &ModuleEntry32));
	}

	CloseHandle(hSnapshot);

	return v;
}


DWORD Cheat::dwGetModuleBaseAddress(LPCSTR lpszModuleName)
{
	DWORD dwModuleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
	MODULEENTRY32 ModuleEntry32 = { 0 };
	ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &ModuleEntry32))
	{
		do
		{
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



void Cheat::Render()
{
	DWORD p1;
	ReadProcessMemory(hProcess, (LPCVOID)(baseAddress), &p1, sizeof(p1), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(p1 + 0x540), &player.GetHealth(), sizeof(player.GetHealth()), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(p1 + 0x14), &p1, sizeof(p1), NULL);

	// Player Position
	ReadProcessMemory(hProcess, (LPCVOID)(p1 + 0x30), &player.GetPosition().x, sizeof(player.GetPosition().x), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(p1 + 0x34), &player.GetPosition().y, sizeof(player.GetPosition().y), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(p1 + 0x38), &player.GetPosition().z, sizeof(player.GetPosition().z), NULL);

	// Camera Position
	ReadProcessMemory(hProcess, (LPCVOID)(clientBase + 0x76F248), &player.GetVerticalCameraPosition(), sizeof(player.GetVerticalCameraPosition()), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(clientBase + 0x76F258), &player.GetHorizontalCameraPosition(), sizeof(player.GetHorizontalCameraPosition()), NULL);

	ReadProcessMemory(hProcess, (LPCVOID)(clientBase + 0x7729C0), &player.GetMoveSpeed(), sizeof(player.GetMoveSpeed()), NULL);
}

void Cheat::Print()
{
	setlocale(LC_ALL, "Russian");

	std::cout << g._copyright << std::endl;
	std::cout << g._update << std::endl;
	//std::cout << g._repository << std::endl;
	std::cout << g._telegram << std::endl;
}

int Cheat::GetRandomNumber(int rMax)
{
	srand((unsigned int)time(NULL));
	return (rand() % rMax + 1);
}

float Cheat::GetPrevRenderHealth()
{
	DWORD p1;

	float health;
	ReadProcessMemory(hProcess, (LPCVOID)(baseAddress), &p1, sizeof(p1), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(p1 + 0x540), &health, sizeof(health), NULL);

	return health;
}

Cheat::~Cheat()
{

}
