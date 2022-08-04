#include "TDBClient.h"
#include "TDBAPI.h"
#include <vector>

using namespace std;

TDBClient::TDBClient()
{
}

bool TDBClient::init(std::string server_ip, std::string user_name, std::string passwd, int port)
{
    const char *svr_ip = server_ip.c_str();
	int svr_port = port;
	const char *username = user_name.c_str();
	const char *password = passwd.c_str();

	OPEN_SETTINGS settings = {0};
	strcpy(settings.szIP,   svr_ip);
	sprintf(settings.szPort, "%d",svr_port);
	strcpy(settings.szUser, username);
	strcpy(settings.szPassword,  password);
	settings.nRetryCount = 100;
	settings.nRetryGap = 100;
	settings.nTimeOutVal = 100;

	TDBDefine_ResLogin LoginRes = {0};

	hTdb = TDB_Open(&settings, &LoginRes);

	if (!hTdb)
	{
		printf("连接失败！");
		getchar();
		return false;
	}
    return true;
}

void TDBClient::get_order(char* szCode, char* szMarket, int nDate, vector<TDBDefine_Order> &vec_orders)
{
	TDBDefine_ReqOrder req = {0};
	strncpy(req.chCode, szCode, sizeof(req.chCode));
	strncpy(req.chMarketKey, szMarket, sizeof(req.chMarketKey));
	req.nDate = nDate;
	req.nBeginTime = 0;
	req.nEndTime = 0;

	int pCount = 0;
	TDBDefine_Order *pOrder = nullptr;
	int ret = TDB_GetOrder(hTdb,&req, &pOrder, &pCount);
	for (int i = 0; i < pCount; ++i)
		vec_orders.push_back(pOrder[i]);
	TDB_Free(pOrder);
}

// vector<TDBDefine_Transaction> TDBClient::get_transaction(char* szCode, char* szMarket, int nDate, vector<TDBDefine_Transaction> &vec_trans)
void TDBClient::get_transaction(char* szCode, char* szMarket, int nDate, vector<TDBDefine_Transaction> &vec_trans)
{
	TDBDefine_ReqTransaction req = {0};
	strncpy(req.chCode, szCode, sizeof(req.chCode));
	strncpy(req.chMarketKey, szMarket, sizeof(req.chMarketKey));
	req.nDate = nDate;
	req.nBeginTime = 0;
	req.nEndTime = 0;

	TDBDefine_Transaction *pTransaction = nullptr;
	int pCount = 0;
	int ret = TDB_GetTransaction(hTdb, &req, &pTransaction, &pCount);
	printf("%d, %d, %lld, %s", pTransaction[0].nDate, pTransaction[0].nTime, pTransaction[0].nTradePrice, pTransaction[0].chWindCode);
	for (int i = 0; i < pCount; ++i)
		vec_trans.push_back(pTransaction[i]);
	TDB_Free(pTransaction);
}

void TDBClient::get_code_table(char* szMarket)
{
	TDBDefine_Code* pCodetable = NULL;
	int pCount;
	int ret = TDB_GetCodeTable(hTdb, szMarket, &pCodetable, &pCount);

	if (ret == TDB_NO_DATA)
	{
		printf("无代码表！");
		return;
	}
	printf("---------------------------Code Table--------------------\n");
	printf("收到代码表项数：%d，\n\n",pCount);

	TDB_Free(pCodetable);
}

int TDBClient::close()
{
	return TDB_Close(hTdb);
}
