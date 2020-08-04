#pragma once
#include "include/curl/curl.h"
#include <string>
#include <algorithm>

#define client Client::GetInstance()

#define ACCOUNT_EXIST			1000
#define ACCOUNT_NOT_EXIST		1001
#define ACCOUNT_NOT_REGISTERED  1004
#define SUCCESS_CONNECTION		1002
#define DENIDED_CONNECTION		1003

#define INITIALIZATION_REQUEST	2000
#define UPDATION_REQUEST		2001
#define SUCCESS_UPDATION		2002

#define GET_DEADLINE_REQUEST	3001
#define GET_CURRENT_DAY_REQUEST	3002
#define GET_BOTSERIAL_REQUEST	3003

class Client
{
private:
	// Необходимо чтобы проверить MD5 консоли
	std::string app_serial;
	std::string bot_serial = "NULL";
	std::string disk_serial;
	//std::string user_name;

	// char * mac_address;
	std::string date;
	std::string dead_line;

public:
	Client();
	~Client();

	CURLcode Send(const char * data);
	std::string Get(const char* data);
	std::string GetDeadline();
	std::string GetCurrentDate();
	std::string GetBotSerial();

	bool Connection();
	void Init(char ** argv);

	static Client & GetInstance()
	{
		static Client _client;
		return _client;
	}
};