#pragma once
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
    size_t threshold_L = 0;  //  Soglia L letta dal file
    size_t exceed_count = 0; // Conta quante volte la coda supera L
    size_t total_observations = 0; // Conta quante volte il server è stato osservato

public:
    Server(real_t t1, real_t t2, size_t L) : process_time_1(t1), process_time_2(t2), threshold_L(L) {}

    void update(Request req) override {
        request_queue.push(req);
        /*
        std::cout << "[Server] New Request: ID=" << req.id << " Type=" << req.type
                  << " Queue Length=" << request_queue.size() << " at Time " << req.init_time << std::endl;
        */
    }

    void process(Time time) {
        total_observations++;  

        if (request_queue.size() > threshold_L) {
            exceed_count++;  
        }

        if (busy_until > time) return;  

        if (!request_queue.empty()) {
            Request current_request = request_queue.front();
            request_queue.pop();

            real_t process_time = (current_request.type == REQUEST_1) ? process_time_1 : process_time_2;
            busy_until = time + process_time;
        }
    }
    // Restituisce la probabilità che la coda superi la soglia L
    real_t get_exceed_probability() const {
        return (total_observations > 0) ? static_cast<real_t>(exceed_count) / total_observations : 0.0;
    }
    
    
    void reset() {
        busy_until = 0;
        while (!request_queue.empty()) request_queue.pop();
        exceed_count = 0;
        total_observations = 0;
    }
};
