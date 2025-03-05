#ifndef PROJECT_MANAGER_HPP
#define PROJECT_MANAGER_HPP

#include <iostream>
#include <vector>
#include <random>
#include "data.hpp"
#include "employee.hpp"
#include "../mocc/buffer.hpp"
#include "../mocc/time.hpp"

class P_Manager : public Observer<Time>, public Notifier<Task, size_t> {
    std::default_random_engine &random_engine;
    std::vector<std::vector<Employee *>> &employees;
    Buffer<Task> *b;

public:
    P_Manager(std::default_random_engine &random_engine, Buffer<Task> *buffer,
              std::vector<std::vector<Employee *>> &employees)
        : random_engine(random_engine), b(buffer), employees(employees) {}

    void update(Time t) override {
        if (!b->is_empty()) {
            Task task = b->pop_front();
            Qualification q = task.q;
            Employee* best_employee = nullptr;
            real_t min_completion_time = std::numeric_limits<real_t>::max();

            // Cerca tra i dipendenti "idle" quello con il tempo atteso minore
            for (Employee* &employee : employees[q - 1]) {
                if (employee->is_idle()) {
                    real_t p_ikq = 0.5 * exp(-(employee->id / (real_t)W + task.k / (real_t)N + q / (real_t)Q));
                    real_t expected_time = 1.0 / (1.0 - p_ikq);  // θ(i, k, q)

                    if (expected_time < min_completion_time) {
                        min_completion_time = expected_time;
                        best_employee = employee;
                    }
                }
            }

            if (!best_employee) {
                // Se nessun dipendente è idle, il task viene rimesso in coda
                if (!b->is_full()) {
                    b->update(task);
                } else {
                    std::cerr << "Errore: buffer pieno, impossibile assegnare il task!\n";
                    std::exit(1);
                }
                return;
            }

            // Assegna il task al miglior dipendente
            notify(task, best_employee->id);
        }
    }
};

#endif
