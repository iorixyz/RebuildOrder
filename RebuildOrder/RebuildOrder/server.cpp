#include <iostream>
#include <list>
#include <map>

#include "TDBClient.h"
#include "zmq.h"
#include "zmq_utils.h"
#include <Windows.h>
#include <thread>
#include "zhelpers.hpp"


using namespace std;


//merge trans and order to one queue
void merge(std::string &wind_code)
{
	// TDBDefine_Transaction *pTrans = NULL;
	// TDBDefine_Order *pOrder = NULL;
	// int nOrderCount = 0;
	// int nTransCount = 0;
	// get_transaction(hTdb, testCode, testMarket, 20220725, pTrans, nTransCount);
	// Sleep(3000);
	// get_order(hTdb, testCode, testMarket, 20220725, pOrder, nOrderCount);

}

//begin to replay handicap
void start(string &wind_code)
{
	
}

//pause replay
void pause(string &wind_code)
{
	
}


void net_thread()
{
	// void *s;
	// // ctx = zmq_init (1);
	// if (!global_ctx)
	// 	cout << "create ctx error" << endl;
	// s = zmq_socket(global_ctx, ZMQ_PULL);
	// if (!s)
	// 	cout << "create socket error" << endl;
	zmq::context_t context(2);

    //  Connect to task ventilator
    zmq::socket_t receiver(context, ZMQ_PULL);
    receiver.connect("tcp://localhost:5557");

    //  Connect to weather server
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5556");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "10001 ", 6);

    //  Initialize poll set
    zmq::pollitem_t items [] = {
        { receiver, 0, ZMQ_POLLIN, 0 },
        { subscriber, 0, ZMQ_POLLIN, 0 }
    };
    //  Process messages from both sockets
    while (1) {
        zmq::message_t message;
        zmq::poll (&items [0], 2, -1);
        
        if (items [0].revents & ZMQ_POLLIN) {
            receiver.recv(&message);
            //  Process task
        }
        if (items [1].revents & ZMQ_POLLIN) {
            subscriber.recv(&message);
            //  Process weather update
        }
    }
}

void prepare_data()
{
	auto tdb = TDBClient();
	bool ret = tdb.init("192.168.80.115", "gaoyu", "123456", 10010);
	if (!ret)
	{
		cout << "init error!" << endl;
		tdb.close();
		return;
	}

	char code[] = "000002.SZ";
	char market[] = "SZ-2-0";
	vector<TDBDefine_Transaction> vec_trans;
	vector<TDBDefine_Order> vec_orders;
	tdb.get_transaction(code, market, 20220725, vec_trans);
	// printf("trans: %d, %d, %lld\n", vec_trans[0].nDate, vec_trans[0].nTime, vec_trans[0].nTradePrice);

	tdb.get_order(code, market, 20220725, vec_orders	);
	map<long long, list<TDBDefine_Order>> map_order_queue;
	for (auto it = vec_orders.begin(); it != vec_orders.end(); ++it)
	{
		if (it->nTime > 92500000)
			break;
		// if (map_order_queue.find(it->nOrderPrice) != map_order_queue.end())
		{
			map_order_queue[it->nOrderPrice].push_back(*it);
		}
		// else
		// {
			// map_order_queue[it->nOrderPrice] = v
		// }
	}
}

int main()
{
	// thread net_t(net_thread);
	thread data_t(prepare_data);
	// HANDLE thread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
	// HANDLE prepare = CreateThread(NULL, 0, GetData, NULL, 0, NULL);
	// net_t.join();
	data_t.join();
	// CloseHandle(thread);
	// CloseHandle(prepare);

	getchar();
	return 0;
}
