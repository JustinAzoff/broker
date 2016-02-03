#include "broker/broker.hh"
#include "broker/endpoint.hh"
#include "broker/message_queue.hh"
#include "testsuite.h"
#include <vector>
#include <set>
#include <poll.h>
#include <iostream>

#include <chrono>
#include <thread>

using namespace std;

int main(int argc, char** argv)
{
    broker::init();

    broker::endpoint sender("sender");
    sender.peer("127.0.0.1", 9999);

    bool ok = false;

    for(int i = 0; i < 1000000; i++) {
        if(ok) {
            sender.send("topic", broker::message{i});
        }

        if(!ok) {
            auto conn_status = sender.outgoing_connection_status().need_pop().front();
            ok = conn_status.status == broker::outgoing_connection_status::tag::established;
            if(ok) {
                std::cout << "established connection to: " << conn_status.peer_name << std::endl;
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        } else if(ok && i % 6000 == 0) {
            std::cout << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            auto conn_status = sender.outgoing_connection_status().want_pop();
            for ( auto cs : conn_status ) {
                if ( cs.status != broker::outgoing_connection_status::tag::established ) {
                    std::cout << "connection error" << std::endl;
                    ok = false;
                }
            }
        }

    }
}
