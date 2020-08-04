#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <stdio.h>
#include <Windows.h>
#include <Assert.h>
#include <Lmcons.h>
#include <mutex>

#define BOT_INIT Bot

class Application
{
private:
	HANDLE mutex;
public:
	Application(char ** _argv);
	~Application();
};