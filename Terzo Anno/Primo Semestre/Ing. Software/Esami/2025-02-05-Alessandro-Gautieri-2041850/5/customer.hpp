#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP
#include "../mocc/time.hpp"
#include "../mocc/observer.hpp"
#include "../mocc/notifier.hpp"
#include "data.hpp"
#include <random>
#include <vector>
#include <iostream>

class Customer : public Observer<Time>, public Notifier<Request> {
    std::vector<std::vector<double>> transition_matrix;
    std::default_random_engine &r_engine;
    std::discrete_distribution<int> state_transition;
    RequestType state;
    Time next_request;
    size_t id = 0;
    size_t served_customers = 0;  // Clienti serviti

public:
    Customer(std::default_random_engine &r_engine, const std::vector<std::vector<double>> &matrix)
        : r_engine(r_engine), transition_matrix(matrix), state(IDLE) {}

    void update(Time time) override {
        if (next_request <= time) {
            state_transition = std::discrete_distribution<int>(transition_matrix[state].begin(), transition_matrix[state].end());
            RequestType previous_state = state;
            state = static_cast<RequestType>(state_transition(r_engine));

            if (state == REQUEST_1 || state == REQUEST_2) {
                notify(Request{time, id++, state});
            }

            if (previous_state != IDLE && state == IDLE) {
                served_customers++;  // Incremento il numero di clienti serviti 
            }

            next_request = time + 1;
        }
    }

    size_t get_served_customers() const { return served_customers; }
    
    void reset() {  
        state = IDLE;
        served_customers = 0;
        id = 0;
        next_request = 1;
    }
};
#endif