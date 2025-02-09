#ifndef SERVER_HPP_
#define SERVER_HPP_
#include "data.hpp"
#include "customer.hpp"
#include "../mocc/observer.hpp"
#include "../mocc/time.hpp"
#include <queue>
#include <iostream>

class Server : public Observer<Request> {
    Time busy_until = 0;
    std::queue<Request> request_queue;
    real_t process_time_1, process_time_2;
    size_t total_queue_length = 0;  
    size_t queue_observations = 0;  

public:
    Server(real_t t1, real_t t2) : process_time_1(t1), process_time_2(t2) {}

    void update(Request req) override {
        request_queue.push(req);
       // std::cout << "[Server] New Request: ID=" << req.id << " Type=" << req.type << " at Time " << req.init_time << std::endl;
    }

    void process(Time time) {
    // Aggiorniamo la lunghezza media della coda
    total_queue_length += request_queue.size();
    queue_observations++;

    /*
    std::cout << "[Server] Time: " << time 
              << " | Queue Size: " << request_queue.size()
              << " | Busy Until: " << busy_until << std::endl;
    */
    if (busy_until > time) return;  

    if (!request_queue.empty()) {
        Request current_request = request_queue.front();
        request_queue.pop();

        real_t process_time = (current_request.type == REQUEST_1) ? process_time_1 : process_time_2;
        busy_until = time + process_time;
    }
}


    real_t get_average_queue_length() const {
    /*
    std::cout << "[DEBUG] Total Queue Length: " << total_queue_length 
              << " | Observations: " << queue_observations << std::endl;
              */
    return (queue_observations > 0) ? static_cast<real_t>(total_queue_length) / queue_observations : 0.0;
}


    void reset() {
        busy_until = 0;
        while (!request_queue.empty()) request_queue.pop();
        total_queue_length = 0;
        queue_observations = 0;
    }
};
#endif