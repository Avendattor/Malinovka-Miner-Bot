#include "Client.h"
#include "System.h"
#include "include/md5.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <clocale>
#include <string>

std::string convertToString(char* a, int size)
{
	int i;
	std::string s = "";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

std::string Client::GetCurrentDate()
{
	char* s_query = (char*)malloc(256);

	//sprintf(s_query, "type_request=%d&app_serial=%s&bot_serial=%s&disk_serial=%s&mac_address=%s" \
	//	, GET_CURRENT_DAY_REQUEST, app_serial.c_str(), bot_serial.c_str(), disk_serial.c_str(), mac_address);

	sprintf(s_query, "type_request=%d&app_serial=%s&bot_serial=%s&disk_serial=%s" \
		, GET_CURRENT_DAY_REQUEST, app_serial.c_str(), bot_serial.c_str(), disk_serial.c_str());
	std::string result = Get(s_query);

	return result;
}
std::string Client::GetBotSerial()
{
	char* s_query = (char*)malloc(256);
	//sprintf(s_query, "type_request=%d&app_serial=%s&bot_serial=%s&disk_serial=%s&mac_address=%s" \
	//	, GET_BOTSERIAL_REQUEST, app_serial.c_str(), bot_serial.c_str(), disk_serial.c_str(), mac_address);
	//char* s_query = (char*)malloc(256);

	sprintf(s_query, "type_request=%d&app_serial=%s&bot_serial=%s&disk_serial=%s" \
		, GET_BOTSERIAL_REQUEST, app_serial.c_str(), bot_serial.c_str(), disk_serial.c_str());
	std::string result = Get(s_query);

	return result;
}
std::string Client::GetDeadline()
{
	char* s_query = (char*)malloc(256);
	//sprintf(s_query, "type_request=%d&app_serial=%s&bot_serial=%s&disk_serial=%s&mac_address=%s" \
	//	, GET_DEADLINE_REQUEST, app_serial.c_str(), bot_serial.c_str(), disk_serial.c_str(), mac_address);

	sprintf(s_query, "type_request=%d&app_serial=%s&bot_serial=%s&disk_serial=%s" \
		, GET_DEADLINE_REQUEST, app_serial.c_str(), bot_serial.c_str(), disk_serial.c_str());

	std::string result = Get(s_query);

	return result;
}

bool Client::Connection()
{
	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	char* s_query = (char*)malloc(256);
	//sprintf(s_query, "type_request=%d&app_serial=%s&bot_serial=%s&disk_serial=%s&mac_address=%s" \
	//	, INITIALIZATION_REQUEST, app_serial.c_str(), bot_serial.c_str(), disk_serial.c_str(), mac_address);

	sprintf(s_query, "type_request=%d&app_serial=%s&bot_serial=%s&disk_serial=%s" \
		, INITIALIZATION_REQUEST, app_serial.c_str(), bot_serial.c_str(), disk_serial.c_str());

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://hots4mp.000webhostapp.com");

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, s_query);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		system("cls");
	}
	
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);


	//CURLcode res = Send(s_query);
	std::string::size_type sz;
	int code = std::stoi(readBuffer, &sz);
	system("cls");

	setlocale(LC_ALL, "Russian");
	switch(code)
	{
		case SUCCESS_CONNECTION:
		{
			// быдло код
			std::string::size_type sz;

			std::string::iterator end_pos_date = std::remove(date.begin(), date.end(), '-');
			date.erase(end_pos_date, date.end());
			int curr_date = stoi(date, &sz);

			dead_line = GetDeadline();
			std::string temp_deadline = dead_line;
			std::string::iterator end_pos_dead_line = std::remove(temp_deadline.begin(), temp_deadline.end(), '-');
			temp_deadline.erase(end_pos_dead_line, temp_deadline.end());
			int curr_dead_line = stoi(temp_deadline, &sz);

			if (curr_date > curr_dead_line)
			{
				std::cout << "* Срок действия активационного ключа истёк.\n";
				Sleep(5000);
				exit(-1);
			}

			std::cout << "Malinovka Miner Bot | v." << BOT_VERSION << std::endl;
			std::cout << "С возвращением!\n" << std::endl;
			std::cout << "[!] Дата окончания действия ключа активации: " << dead_line << std::endl;
			system("pause");

			return true;
			break;
		}
		case DENIDED_CONNECTION:
		{
			std::cout << "[!] Отказано в доступе!" << std::endl;
			Sleep(5000);
			//system("pause");
			exit(-1);
			break;
		}
		case ACCOUNT_NOT_EXIST:
		{
			std::cout << "[!] Данная сборка больше недействительна!" << std::endl;
			Sleep(3000);
			exit(-1);
			break;
		}
		case ACCOUNT_NOT_REGISTERED:
		{
			std::string key;
			std::cout << "Введите ключ активации: "; std::cin >> key;

			if (key == GetBotSerial())
			{
				std::cout << "Ключ активирован!" << std::endl;
				char* s_query = (char*)malloc(256);
				//sprintf(s_query, "type_request=%d&app_serial=%s&bot_serial=%s&disk_serial=%s&mac_address=%s" \
				//	, UPDATION_REQUEST, app_serial.c_str(), bot_serial.c_str(), disk_serial.c_str(), mac_address);

				sprintf(s_query, "type_request=%d&app_serial=%s&bot_serial=%s&disk_serial=%s" \
					, UPDATION_REQUEST, app_serial.c_str(), bot_serial.c_str(), disk_serial.c_str());
				std::string result = Get(s_query);

				//Connection();
				//url_easy_cleanup(curl);
				return true;
			}
			else
			{
				std::cout << "Введён неверный ключ активации!" << std::endl;
				curl_easy_cleanup(curl);
				Sleep(2000);
				exit(-1);
			}
		}
	}
}

void Client::Init(char ** argv)
{
	setlocale(LC_ALL, "Russian");
	MD5 md5;
	char my_md5[64];
	sscanf(md5.digestFile(argv[0]), "%s", my_md5);

	app_serial = convertToString(my_md5, sizeof(my_md5)); 
	std::transform(app_serial.begin(), app_serial.end(), app_serial.begin(), ::toupper);

	// mac_address = System::GetMacAddress();

	disk_serial = System::GetFirstSerialNumber();
	std::string::iterator end_pos = std::remove(disk_serial.begin(), disk_serial.end(), ' ');
	disk_serial.erase(end_pos, disk_serial.end());

	//char username[256 + 1];
	//DWORD username_len = 256 + 1;
	//GetUserName(username, &username_len);
	//user_name = convertToString(username, sizeof(username));
	//std::transform(user_name.begin(), user_name.end(), user_name.begin(), ::toupper);
	//user_name = convertToEnglish(user_name);

	dead_line = GetDeadline();
	date = GetCurrentDate();

}

Client::Client()
{

}

CURLcode Client::Send(const char * data)
{
	CURL* curl;
	curl = curl_easy_init();
		
	curl_easy_setopt(curl, CURLOPT_URL, "http://hots4mp.000webhostapp.com");
		
	//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
	
	std::string readBuffer;
	CURLcode res;
	res = curl_easy_perform(curl);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
	if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		
	curl_easy_cleanup(curl);
	return res;
}

std::string Client::Get(const char * data)
{
	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	char* s_query = (char*)malloc(256);

		curl_easy_setopt(curl, CURLOPT_URL, "http://hots4mp.000webhostapp.com");

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		
		return readBuffer;
}

Client::~Client()
{


}