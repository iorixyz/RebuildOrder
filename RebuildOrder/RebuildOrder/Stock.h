#pragma once
#include <vector>
#include <list>
#include <map>
#include "TDBAPIStruct.h"
#include <string>
#include <deque>


class Stock
{
public:
	Stock();
	~Stock();

public:
	void LoadData(const std::vector<TDBDefine_Order> &vec_orders);
	void BeginReplay();
	void PauseReplay();
	void StopReplay();
	std::string get_name() {return m_stock_name;}
	void set_name(const std::string &name) {m_stock_name = name;}

private:
	std::string m_stock_name;
	std::map<long long, std::deque<TDBDefine_Order>> m_buy_queue;
	std::map<long long, std::deque<TDBDefine_Order>> m_sell_queue;
	long long ask1_price;
	long long bid1_price;
	int ask1_volume;
	int bid1_volume;
};

