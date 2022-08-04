#pragma once
#include "TDBClient.h"
#include <string>
#include <TDBAPIStruct.h>
#include <vector>

class TDBClient
{
public:
	TDBClient();

	~TDBClient()
	{
	}

public:
	bool init(std::string server_ip, std::string user_name, std::string passwd, int port);
	// TDBDefine_Transaction* get_transaction(char* szCode, char* szMarket, int nDate, TDBDefine_Transaction *pTrans, int &pCount);
	// std::vector<TDBDefine_Transaction> get_transaction(char* szCode, char* szMarket, int nDate);
	void get_transaction(char* szCode, char* szMarket, int nDate, std::vector<TDBDefine_Transaction> &vec_trans);
	void get_order(char* szCode, char* szMarket, int nDate, std::vector<TDBDefine_Order> &vec_orders);
	void get_code_table(char* szMarket);
	int close();
private:
	THANDLE hTdb = NULL;
};

