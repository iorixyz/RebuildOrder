// RebuildOrder.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "TDBAPI.h"
#include "iostream"
#include <string>
#include <algorithm>
#include <assert.h>
#ifdef __PLATFORM_LINUX__
#include <unistd.h>
#else
#define _CRT_SECURE_NO_WARNINGS 
#include "Windows.h"
#endif
#include <fstream>
#include <vector>
#include <map>


using namespace std;

#ifdef NDEBUG

#define AssertEx(expr) expr;

#else
#define AssertEx(expr) {int n = (int)(expr); assert(n);} 
#endif

#define ELEMENT_COUNT(arr) (sizeof(arr)/sizeof(*arr))

void get_transaction(THANDLE hTdb, char* szCode, char* szMarket, int nDate, TDBDefine_Transaction *pTrans, int &pCount);
void get_order(THANDLE hTdb, char* szCode, char* szMarket, int nDate, TDBDefine_Order *pOrder, int &pCount);
void get_code_table(THANDLE hTdb, char* szMarket);


std::string int2str(int n)
{
	char szBuf[32];
	_snprintf(szBuf, sizeof(szBuf)/sizeof(szBuf[0]), "%d", n);
	return std::string(szBuf);
}
std::string int64_2_str(const __int64& i)
{
	char szBuf[32];
	_snprintf(szBuf, sizeof(szBuf) / sizeof(szBuf[0]), "%lld", i);
	return std::string(szBuf);
}

std::string array2str(const int* arr, int len)
{
	std::string str;
	for (int i=0; i<len; i++)
	{
		if (i == len-1)
		{
			str += int2str(arr[i]) + " ";
		}
		else 
		{
			str += int2str(arr[i]) + ",";
		}
	}
	return str;
}

std::string array2str(const __int64* arr, int len)
{
	std::string str;
	for (int i = 0; i < len; i++)
	{
		if (i == len - 1)
		{
			str += int64_2_str(arr[i]) + " ";
		}
		else
		{
			str += int64_2_str(arr[i]) + ",";
		}
	}
	return str;
}

static int ErrNum = 0;
static char ErrCode[100][10] = {0};

char testMarket[24] = "SZ-2-0";
char testCode[24]   = "000002.SZ";
THANDLE hTdb = NULL;

struct common_msg
{
	int t; //0 order, 1 trans
	char windcode[32];
	int date;
	int time;
	int price;
	int volume;
	int bsflag; //0 buy, 1 sell, 2 cancel, or other status

};


map<string, vector<common_msg>> global_rebuild_handicap;

//merge trans and order to one queue
void merge(string &wind_code)
{
	TDBDefine_Transaction *pTrans = NULL;
	TDBDefine_Order *pOrder = NULL;
	int nOrderCount = 0;
	int nTransCount = 0;
	get_transaction(hTdb, testCode, testMarket, 20220725, pTrans, nTransCount);
	Sleep(3000);
	get_order(hTdb, testCode, testMarket, 20220725, pOrder, nOrderCount);

		


}

//begin to replay handicap
void start(string &wind_code)
{
	
}

//pause replay
void pause(string &wind_code)
{
	
}

int main()
{
	// THANDLE hTdb = NULL;
	char svr_ip[20] = "192.168.80.115";
	int svr_port = 10010;
	char username[20] = "gaoyu";
	char password[20] = "123456";

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

	int nRet = TDB_SUCCESS;

	if (!hTdb)
	{
		printf("连接失败！");
		getchar();
		return 0;
	}

	// if (testMarket[0])
	// {
	// 	Sleep(3000);
	// 	get_code_table(hTdb, testMarket);
	// }

	strcpy(testCode, "000002.SZ");
	// if (testMarket[0])
	// {
	// 	get_transaction(hTdb, testCode, testMarket, 20220725);//逐笔成交
	// 	Sleep(3000);
	// 	get_order(hTdb, testCode, testMarket, 20220725);//逐笔委托
	// }
}


void get_code_table(THANDLE hTdb, char* szMarket)
{
	TDBDefine_Code* pCodetable = NULL;
	int pCount;
	bool outPutTable = false;
	int ret = TDB_GetCodeTable(hTdb, szMarket, &pCodetable, &pCount);

	if (ret == TDB_NO_DATA)
	{
		printf("无代码表！");
		return;
	}
	printf("---------------------------Code Table--------------------\n");
	printf("收到代码表项数：%d，\n\n",pCount);
	if (pCodetable && pCount>0)
	{
		memcpy(testCode, pCodetable[0].chWindCode, sizeof(testCode)-1);
	}
	//输出
	if(outPutTable)
	{
		for (int i=0;i<pCount;i++)
		{
			printf("交易所代码 chWindCode:%s \n", pCodetable[i].chCode);			
			printf("市场代码 chWindCode:%s \n", pCodetable[i].chMarket);
			printf("证券中文名称 chWindCode:%s \n", pCodetable[i].chCNName);
			printf("证券英文名称 chWindCode:%s \n", pCodetable[i].chENName);
			printf("证券类型 chWindCode:%d \n", pCodetable[i].nType);
			printf("当日编号 nRecord:%d \n", pCodetable[i].nRecord);//当日代码nRecord>=2,历史代码nRecord=0,-1表示服务器未支持
			printf("----------------------------------------\n");
		}
	}
	else
	{
		//print top 10
		int i = pCount>10 ? 10 :pCount;
		for (; i > 0 ;i--)
		{
			printf("交易所代码 chWindCode:%s \n", pCodetable[i-1].chCode);	
			printf("市场代码 chWindCode:%s \n", pCodetable[i-1].chMarket);
			printf("证券中文名称 chWindCode:%s \n", pCodetable[i-1].chCNName);
			printf("证券英文名称 chWindCode:%s \n", pCodetable[i-1].chENName);
			printf("证券类型 chWindCode:%d \n", pCodetable[i-1].nType);
			printf("----------------------------------------\n");
		}
	}
	//释放
	TDB_Free(pCodetable);
}

void get_transaction(THANDLE hTdb, char* szCode, char* szMarketKey, int nDate, TDBDefine_Transaction *pTransaction, int &pCount)
{
	//请求
	TDBDefine_ReqTransaction req = {0};
	strncpy(req.chCode, szCode, sizeof(req.chCode));
	strncpy(req.chMarketKey, szMarketKey, sizeof(req.chMarketKey));
	req.nDate = nDate;
	req.nBeginTime = 0;
	req.nEndTime = 0;

	// TDBDefine_Transaction *pTransaction = NULL;
	// int pCount;
	int ret = TDB_GetTransaction(hTdb,&req, &pTransaction, &pCount);


#if 1
	// printf("---------------------------------------Transaction Data------------------------------------------\n");
	// printf("收到 %d 条逐笔成交消息，打印 1/10000 条\n", pCount);
	// std::ofstream trans_file("transaction.csv");
	// trans_file << "windcode,code,date,time,index,function_code,order_kind,bsflag,price,volume,askorder,bidorder" << endl;
	// for (int i=0; i<pCount; ++i)
	// {
		// const TDBDefine_Transaction& trans = pTransaction[i];
		// trans_file << trans.chWindCode << "," << trans.chCode << "," << trans.nDate << "," << trans.nTime <<
			// trans.nIndex << "," << trans.chFunctionCode << "," << trans.chOrderKind << "," << trans.chBSFlag <<
			// "," << trans.nTradePrice << "," << trans.nTradeVolume << "," << trans.nAskOrder << "," << trans.nBidOrder <<
			// endl;
	// }
#endif
	//释放
	// TDB_Free(pTransaction);
}

//逐笔委托
void get_order(THANDLE hTdb, char* szCode, char* szMarketKey, int nDate, TDBDefine_Order *pOrder, int &pCount)
{
	//请求
	TDBDefine_ReqOrder req = {0};
	strncpy(req.chCode, szCode, sizeof(req.chCode));
	strncpy(req.chMarketKey, szMarketKey, sizeof(req.chMarketKey));
	req.nDate = nDate;
	req.nBeginTime = 0;
	req.nEndTime = 0;

	// TDBDefine_Order *pOrder = NULL;
	// int pCount;
	int ret = TDB_GetOrder(hTdb,&req, &pOrder, &pCount);
	printf("---------------------Order Data----------------------\n");
	printf("收到 %d 条逐笔委托消息，打印 1/10000 条\n", pCount);

	// std::ofstream order_file("orders.csv");
	// order_file << "windcode,code,date,time,index,order_index,order_kind,function_code,price,volume,channel,origin_no,biz_index" << endl;
	// for (int i=0; i<pCount; ++i)
	// {
		// const TDBDefine_Order& order = pOrder[i];

		// order_file << order.chWindCode << "," << order.chCode << "," <<
			// order.nDate << "," << order.nTime << "," << order.nIndex << "," << order.nOrder <<
			// "," << order.chOrderKind << "," << order.chFunctionCode << "," << order.nOrderPrice <<
			// "," << order.nOrderVolume << "," << order.nChannel << "," << order.nOrderOriNo << "," <<
			// order.nBizIndex << endl;
	// }

	// order_file.close();
	//释放
	// TDB_Free(pOrder);
}
