#pragma once
#include <Windows.h>
#include <stdio.h>
#include <cstdlib>
#include <string>

#define BOT_VERSION			200421
class System
{
public:
	static char * GetMacAddress();
	static std::string GetFirstSerialNumber();
};