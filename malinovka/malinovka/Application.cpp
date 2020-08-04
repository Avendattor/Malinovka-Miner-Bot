#include "Application.h"
#include "Client.h"
#include "MinerBot.h"

#define  _DEBUG_MODE
Application::Application(char ** argv)
{
	mutex = CreateMutex(NULL, true, "MutexOfTheMyApplication");
	DWORD mutexResult = WaitForSingleObject(mutex, 0);

	if (mutexResult != WAIT_OBJECT_0)
	{
		exit(-1);
	}

#ifndef _DEBUG_MODE
	try {
		client.Init(argv);
		if (client.Connection())
		{
			BOT_INIT;
		}
	}
	catch (const char* msg) {
		std::cerr << msg << std::endl;
	}

#else
	BOT_INIT;
#endif


	system("pause");
}

Application::~Application()
{
	ReleaseMutex(mutex);
	//И удаляем его.
	CloseHandle(mutex);
}