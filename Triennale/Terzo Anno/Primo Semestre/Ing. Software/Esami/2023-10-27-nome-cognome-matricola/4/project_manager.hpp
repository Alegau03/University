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
            std::vector<int> idle_employees;

            // Cerca solo dipendenti "idle" con la qualifica corretta
            for (Employee *&employee : employees[q - 1]) {
                if (employee->phase == 0) { // Solo quelli idle
                    idle_employees.push_back(employee->id);
                }
            }

            if (idle_employees.empty()) {
                // Se nessun dipendente idle è disponibile, il task viene rimesso in coda
                if (!b->is_full()) {
                    b->update(task);
                } else {
                    std::cerr << "Errore: buffer pieno, impossibile assegnare il task!\n";
                    std::exit(1);
                }
                return;
            }

            // Se ci sono dipendenti idle, scegli uno a caso tra loro
            size_t bound = idle_employees.size() - 1;
            std::uniform_int_distribution<> random_idle_employee(0, bound);
            size_t id = idle_employees[random_idle_employee(random_engine)];
            notify(task, id);
        }
    }
};

#endif
