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

const auto padding = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijkl";

int main(int argc, char** argv)
{
    broker::init();

    broker::endpoint sender("sender");
    sender.peer("127.0.0.1", 9999);

    bool ok = false;
    while(!ok) {
        std::cout << "waiting for connection..." << endl;
        auto conn_status = sender.outgoing_connection_status().need_pop().front();
        ok = conn_status.status == broker::outgoing_connection_status::tag::established;
    }
    std::cout << "Connected!" << endl;

    for(int i = 0; i < 1000000; i++) {
        sender.send("topic", broker::message{padding, i});
        if (i % 6000 == 0) {
            std::cout << i << std::endl;
            //std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
}
