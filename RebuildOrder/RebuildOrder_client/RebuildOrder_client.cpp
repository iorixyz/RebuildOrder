#include "zmq.h"
#include "zmq_utils.h"
#include <Windows.h>
#include <thread>
#include <iostream>
#include "zhelpers.hpp"


using namespace std;

void parse_quote_msg(zmq::message_t &msg)
{
	
}

void parse_entrust(zmq::message_t &msg)
{
	
}

void net_thread()
{
	zmq::context_t context(2);

    zmq::socket_t receiver(context, ZMQ_REQ);
    receiver.connect("tcp://localhost:5557");

    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5556");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "10001 ", 6);

    zmq::pollitem_t items [] = {
        { receiver, 0, ZMQ_POLLIN, 0 },
        { subscriber, 0, ZMQ_POLLIN, 0 }
    };

	while (true) {
        zmq::message_t message;
        zmq::poll (&items [0], 2, -1);
        
        if (items [0].revents & ZMQ_POLLIN) {
            receiver.recv(&message);
            parse_quote_msg(message);
        }
        if (items [1].revents & ZMQ_POLLIN) {
            subscriber.recv(&message);
            parse_quote_msg(message);
        }
    }
}

int main()
{
	thread net_t(net_thread);

    net_t.join();

    return 0;
}

