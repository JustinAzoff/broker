#include "broker/broker.hh"
#include "broker/endpoint.hh"
#include "broker/message_queue.hh"
#include "testsuite.h"
#include <vector>
#include <set>
#include <poll.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{

    std::chrono::time_point<std::chrono::system_clock> start, end;
    broker::init();

    broker::endpoint listener("listener");
    listener.listen(9999, "127.0.0.1");
    broker::message_queue my_queue("", listener);
    int i = 0;
    start = std::chrono::system_clock::now();
    for(;;) {
        for ( auto& msg : my_queue.need_pop() ) {
            i++;
            if (i % 50000 == 0) {
                end = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_seconds = end-start;
                cout << "Received " << broker::to_string(msg) << " duration: " << elapsed_seconds.count() << " rate: " << 50000/elapsed_seconds.count() << "/sec" << endl;
                start = std::chrono::system_clock::now();
            }
        }
    }
}
