#ifndef EMPLOYEE_HPP
#define EMPLOYEE_HPP

#include "data.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include "../mocc/time.hpp"
#include "../mocc/stat.hpp"

class Employee : public Observer<Task, size_t>,
                 public Observer<Time>,
                 public Notifier<Task, Time> {

    std::default_random_engine &random_engine;
    real_t working_time = 0;
    real_t total_time = 0;

public:
    Qualification q;
    size_t id;
    size_t phase;
    Task actual_task;
    std::vector<std::discrete_distribution<>> transition_matrix;
    Stat working_percentage;

    Employee(std::default_random_engine &random_engine, Qualification q, size_t id)
        : random_engine(random_engine), q(q), id(id), phase(0) {
        transition_matrix = std::vector<std::discrete_distribution<>>(N + 1);
        
        for (size_t k = 1; k <= N; k++) {
            std::vector<double> p(N + 1, 0); 
            p[k] = .5 * exp(-(id / (double)W + k / (double)N + q / (double)Q));
            p[0] = 1 - p[k];  
            transition_matrix[k] = std::discrete_distribution<>(p.begin(), p.end());
        }
    }

    void update(Time t) override {
        total_time += T;

        if (phase != 0) {  
            working_time += T;
            phase = transition_matrix[phase](random_engine);  

            if (phase == 0) {  
                notify(actual_task, t);
                actual_task = {0, 0, 0}; 
            }
        }

        if (total_time > 0) {
            real_t percentage = (working_time / total_time) * 100;
            working_percentage.save(percentage);
        }
    }

    void update(Task t, size_t i) override {
        if (id == i && q == t.q && is_idle()) {
            phase = t.k;
            actual_task = t;
        }
    }

    bool is_idle() const { return phase == 0; }

    real_t mean_working_percentage() const {
        return working_percentage.mean();
    }

    real_t stddev_working_percentage() const {
        return working_percentage.stddev_welford();
    }
};

#endif
