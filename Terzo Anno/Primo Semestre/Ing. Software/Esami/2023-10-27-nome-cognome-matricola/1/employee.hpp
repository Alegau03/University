#ifndef EMPLOYEE_HPP
#define EMPLOYEE_HPP

#include "data.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include "../mocc/time.hpp"

class Employee : public Observer<Task, size_t>,
                 public Observer<Time>,
                 public Notifier<Task, Time> {

    std::default_random_engine &random_engine;

public:
    Qualification q;
    size_t id;
    size_t phase;
    Task actual_task;
    std::vector<std::discrete_distribution<>> transition_matrix;

    Employee(std::default_random_engine &random_engine, Qualification q, size_t id)
        : random_engine(random_engine), q(q), id(id), phase(0) {  // Impostare il dipendente inizialmente su idle
        transition_matrix = std::vector<std::discrete_distribution<>>(N + 1);
        
        for (size_t k = 1; k <= N; k++) {
            std::vector<double> p(N + 1, 0); // Vettore di probabilità
            p[k] = .5 * exp(-(id / (double)W + k / (double)N + q / (double)Q)); // Probabilità di restare occupato
            p[0] = 1 - p[k]; // Probabilità di completare il task
            transition_matrix[k] = std::discrete_distribution<>(p.begin(), p.end()); // Distribuzione discreta
        }
    }

    // Metodo per aggiornare lo stato del dipendente nel tempo
    void update(Time t) override {
        if (phase != 0) {  
            phase = transition_matrix[phase](random_engine);  // Aggiorna lo stato del task in esecuzione
            
            if (phase == 0) {  // Task completato
                notify(actual_task, t); // Notifica che il task è stato completato
                
                // Resetta lo stato
                actual_task = {0, 0, 0}; 
            }
        }
    }

    // Metodo per ricevere un nuovo task
    void update(Task t, size_t i) override {
        if (id == i && q == t.q && phase == 0) { // Accetta il task solo se è idle
            phase = t.k; 
            actual_task = t;
        }
    }
};

#endif
