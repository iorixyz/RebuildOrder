#include "Stock.h"

using namespace std;

Stock::Stock()
{
}

Stock::~Stock()
{
}

void Stock::LoadData(const std::vector<TDBDefine_Order>& vec_orders)
{
	for (auto it = vec_orders.begin(); it != vec_orders.end(); ++it)
	{
		if (it->nTime < 93000000)
			continue;

		if (it->chFunctionCode == 'B')
		{
			//buy
			if (it->nOrderPrice >= ask1_price)
			{
				//trade
				bid1_price = it->nOrderPrice;
			}

			m_buy_queue[it->nOrderPrice].push_back(*it);
		}
		else if (it->chFunctionCode == 'S')
		{
			//sell
			if (it->nOrderPrice >= bid1_price)
			{
				//trade

			}
			m_sell_queue[it->nOrderPrice].push_back(*it);
		}
		else
		{
			//cancel

		}
	}
}
