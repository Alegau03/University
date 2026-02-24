#ifndef SERVER_HPP_
#define SERVER_HPP_
#include "data.hpp"
#include "customer.hpp"
#include "../mocc/observer.hpp"
#include "../mocc/time.hpp"
#include <queue>
#include <unordered_set>  // Per tracciare i clienti serviti unici
#include <iostream>

class Server : public Observer<Request> {
    Time busy_until = 0;
    std::queue<Request> request_queue;
    real_t process_time_1, process_time_2;
    std::unordered_set<size_t> served_customers;  // Traccia clienti unici

public:
    Server(real_t t1, real_t t2) : process_time_1(t1), process_time_2(t2) {}

    void update(Request req) override {
        request_queue.push(req);
        // std::cout << "[Server] New Request: ID=" << req.id << " Type=" << req.type << " at Time " << req.init_time << std::endl;
    }

    void process(Time time) {
        if (busy_until > time) return;  // Il server è ancora occupato

        if (!request_queue.empty()) {
            Request current_request = request_queue.front();
            request_queue.pop();

            real_t process_time = (current_request.type == REQUEST_1) ? process_time_1 : process_time_2;
            busy_until = time + process_time;  

            // Conta il cliente solo se è la prima volta che viene servito
            served_customers.insert(current_request.id);
        }
    }

    bool is_busy(Time time) const {
        return busy_until > time;
    }

    size_t get_processed_customers() const {
        return served_customers.size();  // Restituisce il numero di clienti unici serviti
    }
};
#endif