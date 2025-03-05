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
            std::vector<int> free_employee;

            // Cerca dipendenti liberi con la qualifica giusta
            for (Employee *&employee : employees[q - 1]) {
                if (employee->phase == 0) {
                    free_employee.push_back(employee->id);
                }
            }

            if (free_employee.empty()) {
                // Se nessun dipendente è libero, prova a reinserire il task con priorità
                if (!b->is_full()) {
                    b->update(task);  // Rimetti il task nel buffer per riprovarci dopo
                } else {
                    std::cerr << "Errore: buffer pieno, impossibile assegnare il task!\n";
                    std::exit(1);
                }
                return;
            }

            // Se ci sono dipendenti disponibili, assegna il task a uno di loro
            size_t bound = free_employee.size() - 1;
            std::uniform_int_distribution<> random_employee(0, bound);
            size_t id = free_employee[random_employee(random_engine)];
            notify(task, id);
        }
    }
};

#endif
