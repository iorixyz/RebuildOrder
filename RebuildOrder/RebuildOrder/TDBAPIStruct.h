
#ifndef _TDBAPISTRUCT_H_
#define _TDBAPISTRUCT_H_

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64) || defined(_WINDOWS)
#define __PLATFORM_WINDOWS__
#else
#define __PLATFORM_LINUX__
#endif


#ifdef __PLATFORM_LINUX__
#define __int64 long long
#endif


#pragma  pack(push)
#pragma pack(1)
///基本声明与定义////////////////////////////////////////////////////
//TDB句柄

#ifndef THANDLE
typedef void* THANDLE;
#endif

enum TDB_ERROR
{
    TDB_SUCCESS = 0,

    TDB_NETWORK_ERROR=-1,       //网络错误
    TDB_NETWORK_TIMEOUT=-2,     //网络超时
    TDB_NO_DATA = -3,           //没有数据
    TDB_OUT_OF_MEMORY=-4,       //内存耗尽
    TDB_LOGIN_FAILED = -5,      //登陆失败
    TDB_INVALID_PARAMS=-11,     //无效的参数
    TDB_INVALID_CODE_TYPE,      //无效的代码类型，比如向TDB_GetFuture传入非期货类型代码，返回之。
    
    
    TDB_WRONG_FORMULA=-50,      //指标公式错误        
	TDB_NO_AUTHORIZED_MARKET=-998,					//无授权市场
	TDB_NO_CODE_TABLE=-997,							//无代码表
};



enum TDB_PROXY_TYPE
{
	TDB_PROXY_SOCK4,
	TDB_PROXY_SOCK4A,
	TDB_PROXY_SOCK5,
	TDB_PROXY_HTTP11,
};

struct TDB_PROXY_SETTING
{
	TDB_PROXY_TYPE nProxyType;
	char szProxyHostIp[64];
	char szProxyPort[8];
	char szProxyUser[32];
	char szProxyPwd[32];
};

struct OPEN_SETTINGS
{
    char szIP[24];              //服务器ip地址
    char szPort[8];             //服务器端口
    char szUser[32];            //用户名
    char szPassword[32];        //密码

    unsigned int nTimeOutVal;   //获取数据时，指定网络超时（秒数，为0则设为2分钟），若超过nTimeOutVal秒后未收到回应数据包，则内部会关闭连接
    unsigned int nRetryCount;   //获取数据时，若掉线，指定重连次数（为0则不重连），若重连nRetryCount次之后仍掉线，则返回网络错误
    unsigned int nRetryGap;     //掉线之后重连的时间间隔（秒数，为0则设为1秒）
};

//复权方式
typedef enum REFILLFLAG
{
    REFILL_NONE = 0,        //不复权
    REFILL_BACKWARD=1,      //全程向前复权（从现在向过去）
    REFILL_FORWARD=2,       //全程向后复权（从过去向现在）
} ;

typedef enum CYCTYPE
{
    CYC_SECOND,
    CYC_MINUTE,
    CYC_DAY,
    CYC_WEEK,
    CYC_MONTH,
    CYC_SEASON,
    CYC_HAFLYEAR,
    CYC_YEAR,
    CYC_TICKBAR,
};

//买卖方向
typedef enum ORDERSIDE
{
    ORDERSIDE_BID = 0,      //买
    ORDERSIDE_ASK,          //卖
} ;


///系统消息结构体////////////////////////////////////////////////////

//登录回报
struct TDBDefine_ResLogin
{
    char szInfo[64];             //信息
    int  nMarkets;               //支持市场个数
    char szMarket[256][24];       //市场标志(SZ, SH, CF, SHF, DCE, CZC,...)
    int  nDynDate[256];          //动态数据日期
};

///基本消息结构体////////////////////////////////////////////////////
struct TDBDefine_Code
{
    char chWindCode[32];        //万得代码(ag1312.SHF)
    char chCode[32];            //交易所代码(ag1312)
    char chMarket[24];           //市场代码(SHF-1-0)
    char chCNName[32];          //证券中文名称
    char chENName[32];          //证券英文名称
    int  nType;                 //证券类型
	int  nRecord;               //当日编号(当日代码nRecord>=2,历史代码nRecord=0,-1表示服务器未支持)
};

///行情消息结构体////////////////////////////////////////////////////
//K线数据 KLine
struct TDBDefine_ReqKLine
{
    char chCode[32];            //证券万得代码(ag1312.SHF)
	char chMarketKey[24];		//市场设置,如：SH-1-0;SZ-2-0
    REFILLFLAG nCQFlag;         //除权标志：不复权，向前复权，向后复权
    int nCQDate;                //复权日期(=0:全程复权) 格式：YYMMDD，例如20130101表示2013年1月1日
    int nQJFlag;                //全价标志(债券)(0:净价 1:全价)
    CYCTYPE nCycType;           //数据周期：秒线、分钟、日线、周线、月线、季线、半年线、年线、TickBar
    int nCycDef;                //周期数量：仅当nCycType取值：秒、分钟、日线、周线、月线时，这个字段有效。
    int nAutoComplete;          //自动补齐：仅1秒钟线、1分钟线支持这个标志，（1：补齐；0：不补齐）
    int nBeginDate;             //开始日期(交易日,0:从今天开始)
    int nEndDate;               //结束日期(交易日,=0:跟nBeginDate一样) 
    int nBeginTime;             //开始时间，=0表示从开始，格式：（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
    int nEndTime;               //结束时间，=0表示到结束，格式：（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
};

struct TDBDefine_KLine
{
    char chWindCode[32];            //万得代码(ag1312.SHF)
    char chCode[32];                //交易所代码(ag1312)
    int nDate;                      //日期（自然日）格式：YYMMDD，例如20130101表示2013年1月1日，0表示今天
    int nTime;                      //时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
	__int64    nOpen;               //开盘((a double number + 0.00005) *10000)
	__int64    nHigh;               //最高((a double number + 0.00005) *10000)
	__int64    nLow;                //最低((a double number + 0.00005) *10000)
	__int64    nClose;              //收盘((a double number + 0.00005) *10000)
    __int64	   iVolume;             //成交量
    __int64    iTurover;            //成交额(元)
    int    nMatchItems;             //成交笔数
    int nInterest;                  //持仓量(期货)、IOPV(基金)、利息(债券)
};

//深圳新债券字段
struct TDB_NB_DATA
{
	__int64			iAuctionVolumeTrade;	//匹配成交成交量
	__int64			iAuctionValueTrade;		//匹配成交成交金额
	__int64			iPriceAuctionTrade;		//匹配成交最近价
	char			chTradingPhaseCode[5];	//交易方式所处的交易阶段代码:    N0=匹配成交 N1=协商成交 N2=点击成交 N3=询价成交 N4=竞买成交
	unsigned char	uTradingType;			//当前最新价对应的交易方式:	1=匹配成交 2=协商成交 3=点击成交 4=询价成交 5=竞买成交
	short			nTypeID;				//行情类别, 深交所对应MDStreamID的整数
};

struct TDBDefine_Tick
{
    char chWindCode[32];                //万得代码(ag1312.SHF)
    char chCode[32];                    //交易所代码(ag1312)
    int nDate;                          //日期（自然日）
    int nTime;                          //时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
	__int64 nPrice;                     //成交价((a double number + 0.00005) *10000)
    __int64 iVolume;                    //成交量
    __int64    iTurover;                //成交额(元)
    int nMatchItems;                    //成交笔数
    int nInterest;                      //IOPV(基金)、利息(债券)
    char chTradeFlag;                   //成交标志
    char chBSFlag;                      //BS标志
    __int64 iAccVolume;                 //当日累计成交量
    __int64    iAccTurover;             //当日成交额
	__int64 nHigh;                      //最高((a double number + 0.00005) *10000)
	__int64 nLow;                       //最低((a double number + 0.00005) *10000)
	__int64    nOpen;                   //开盘((a double number + 0.00005) *10000)
	__int64    nPreClose;               //前收盘((a double number + 0.00005) *10000)
	
	//期货字段
	__int64 nSettle;                //结算价((a double number + 0.00005) *10000)
	int nPosition;                  //持仓量
	int nCurDelta;                  //虚实度
	__int64 nPreSettle;             //昨结算((a double number + 0.00005) *10000)
	int nPrePosition;               //昨持仓

	//买卖盘字段
	__int64 nAskPrice[10];			//叫卖价((a double number + 0.00005) *10000)
	__int64 nAskVolume[10];			//叫卖量
	__int64 nBidPrice[10];			//叫买价((a double number + 0.00005) *10000)
	__int64 nBidVolume[10];			//叫买量
	int     nAskOrders[10];			//委卖订单数量
	int     nBidOrders[10];			//委买订单数量
	__int64 nAskAvPrice;			//加权平均叫卖价(上海L2)((a double number + 0.00005) *10000)
	__int64 nBidAvPrice;			//加权平均叫买价(上海L2)((a double number + 0.00005) *10000)
	__int64 iTotalAskVolume;        //叫卖总量(上海L2)
	__int64 iTotalBidVolume;        //叫买总量(上海L2)
   
	//下面的字段指数使用
	__int64 nIndex;                 //不加权指数
    int     nStocks;                //品种总数
    int     nUps;                   //上涨品种数
    int     nDowns;                 //下跌品种数
    int     nHoldLines;             //持平品种数

	__int64	nAvgPrice;				//期货均价，仅郑商所有效
	
	__int64 iAfterPrice;			//盘后价格(科创板有使用到)
	int		nAfterVolume;			//盘后量(科创板有使用到)
	__int64 iAfterTurnover;			//盘后成交金额(科创板有使用到)
	int		nAfterMatchItems;		//盘后成交笔数(科创板有使用到)

	__int64	nHighLimit;				// 涨停价
	__int64	nLowLimit;				// 跌停价

	TDB_NB_DATA NBData;				//深圳新债券字段
};

struct TDBDefine_ReqTick
{
    char chCode[32];			//证券万得代码(ag1312.SHF)
	char chMarketKey[24];		//市场设置,如：SH-1-0;SZ-2-0
    int  nDate;					//请求日期（交易日）,为0则从今天，格式：YYMMDD，例如20130101表示2013年1月1日
    int  nBeginTime;			//开始时间：若0则从头，格式：（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
    int  nEndTime;				//结束时间：若0则至最后

	int nAutoComplete;			//自动补齐标志:( 0：不自动补齐，1:自动补齐）,暂不支持
};


//逐笔成交数据 Transaction
struct TDBDefine_ReqTransaction
{
    char chCode[32];            //证券万得代码(ag1312.SHF)
	char chMarketKey[24];		//市场设置,如：SH-1-0;SZ-2-0
    int  nDate;					//开始日期（交易日），格式YYMMDD
    int  nBeginTime;            //开始时间:0表示从0开始，格式：HHMMSSmmm
    int  nEndTime;              //结束时间：0表示到最后
};

struct TDBDefine_Transaction_EX
{
	int		nTypeID;				//行情类别, 深交所对应MDStreamID的整数
	unsigned char uSettlPeriod;		//结算周期
	unsigned char uSettlType;		//结算方式
	unsigned char uBidExecInstType;	//竞买成交方式
	char	chSecondaryOrderID[16];	//竞买场次编号
	__int64	nMarginPrice;			//达成成交的边际价格
};

struct TDBDefine_Transaction
{
    char    chWindCode[32];     //万得代码(ag1312.SHF)
    char    chCode[32];         //交易所代码(ag1312)
    int     nDate;              //日期（自然日）格式:YYMMDD
    int     nTime;              //成交时间(精确到毫秒HHMMSSmmm)
    int     nIndex;             //成交编号
    char    chFunctionCode;     //成交代码: 'C', 0
    char    chOrderKind;        //委托类别
    char    chBSFlag;           //BS标志
	__int64     nTradePrice;    //成交价格((a double number + 0.00005) *10000)
    int     nTradeVolume;       //成交数量
    int     nAskOrder;          //叫卖序号
    int     nBidOrder;          //叫买序号
	int		nChannel;			//channel id
	__int64 nBizIndex;			//业务编号(上交所)

	int		nExFlag;			//扩展字段是否有效。=1为有效，目前仅深圳新债券有
	TDBDefine_Transaction_EX exData; //扩展字段，目前仅深圳新债券有效
};

//逐笔委托数据
typedef struct TDBDefine_ReqTransaction TDBDefine_ReqOrder ;

struct TDBDefine_Order_EX
{
	int		nTypeID;				//行情类别, 深交所对应MDStreamID的整数
	unsigned char uSettlPeriod;		//结算周期
	unsigned char uSettlType;		//结算方式
	unsigned char uBidTransType;	//竞买业务类别
	unsigned char uBidExecInstType;	//竞买成交方式
	char	chSecondaryOrderID[16];	//竞买场次编号
	char	chQuoteID[12];			//报价消息编号，仅适用于点击成交行情
	char	chMemberID[8];			//交易商代码	
	char	chInvestorType[4];		//交易主体类型
	char	chInvestorID[12];		//交易主体代码	
	char	chInvestorName[124];	//客户名称
	char	chTraderCode[12];		//交易员代码
	char	chMemo[164];			//备注
	__int64	nHighLimitPrice;		//价格上限
	__int64	nLowLimitPrice;			//价格下限
	__int64	nMinQty;				//最低成交数量
	int		nTradeDate;				//交易日期
};

struct TDBDefine_Order
{
    char chWindCode[32];        //万得代码(ag1312.SHF)
    char chCode[32];            //交易所代码(ag1312)
    int  nDate;                 //日期（自然日）格式YYMMDD
    int  nTime;                 //委托时间（精确到毫秒HHMMSSmmm）
    int  nIndex;                //委托编号
    int  nOrder;                //交易所委托号
    char chOrderKind;           //委托类别
    char chFunctionCode;        //委托代码, B, S, C
	__int64  nOrderPrice;       //委托价格((a double number + 0.00005) *10000)
    int  nOrderVolume;          //委托数量
	int	 nChannel;				//channel id
	__int64 nOrderOriNo;		//原始订单号(上交所)
	__int64 nBizIndex;			//业务编号(上交所)

	int		nExFlag;			//扩展字段是否有效。=1为有效，目前仅深圳新债券有
	TDBDefine_Order_EX exData;  //扩展字段，目前仅深圳新债券有效
};

//委托队列数据 OrderQueue
typedef struct TDBDefine_ReqTransaction TDBDefine_ReqOrderQueue;

struct TDBDefine_OrderQueue
{
    char    chWindCode[32];         //万得代码(ag1312.SHF)
    char    chCode[32];             //交易所代码(ag1312)
    int     nDate;                  //日期（自然日）格式YYMMDD
    int     nTime;                  //订单时间(精确到毫秒HHMMSSmmm)
    int     nSide;                  //买卖方向('B':Bid 'A':Ask)
	__int64     nPrice;             //成交价格((a double number + 0.00005) *10000)
    int     nOrderItems;            //订单数量
    int     nABItems;               //明细个数
    int     nABVolume[50];          //订单明细
};

struct TDBDefine_AddFormulaRes
{
    int nErrLine;           //错误行
    char chText[132];       //错误行指标公式正文
    char chInfo[68];        //错误信息
};

struct TDBDefine_FormulaItem
{
    char chFormulaName[28];    //指标公式名称
    char chParam[132];         //参数
};

struct TDBDefine_DelFormulaRes
{
    char chFormularName[28];    //删除的指标名称
    char chInfo[68];            //删除信息
};

struct TDBDefine_CalcFormulaRes
{
    char chWindCode[32];        //万得代码(ag1312.SHF)
    char chCode[32];            //交易所代码(ag1312)

    int nRecordCount;           //记录数量
    int nFieldCount;            //字段数量，最多50个

    char chFieldName[50][28];   //字段名称，
    int* dataFileds[50];        //最多50个字段，如果有日期和时间字段，总是排在前面
};


//请求计算指标公式
struct TDBDefine_ReqCalcFormula
{
    char chFormulaName[28]; //公式名称
    char chParam[32];       //参数, "N1=5,N2=10, N3='000001'
    char chCode[32];        //万得代码，形如：AG1312.SHF
	char chMarketKey[24];		//市场设置,如：SH-1-0;SZ-2-0

    CYCTYPE  nCycType;      //数据周期
    int  nCycDef;           //数据周期定义

    REFILLFLAG  nCQFlag;    //除权标志
    int nBondDataType;      //债券全价标志

    int  nCQDate;           //除权日期
    int  nCalcMaxItems;     //计算的最大数据量
    int  nResultMaxItems;   //传送的结果最大数据量
};

#pragma pack(pop)

#endif//_TDBAPISTRUCT_H_
