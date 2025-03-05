#ifndef TASK_GENERATOR_HPP
#define TASK_GENERATOR_HPP

#include "data.hpp"
#include "../mocc/buffer.hpp"
#include "../mocc/time.hpp"
#include <random>
#include <iostream>

class TaskGenerator : public Observer<Time>, public Notifier<Task> {
    std::default_random_engine &random_engine;
    std::bernoulli_distribution generate_task_prob;
    Buffer<Task> *buffer;
    size_t project_id = 1;
    size_t current_phase = 1;

  public:
    TaskGenerator(std::default_random_engine& random_engine, Buffer<Task> *buffer)
        : random_engine(random_engine), generate_task_prob(p), buffer(buffer) {}
    
    void update(Time t) override {
        if (buffer->is_full()) {
        std::cerr << "Errore: buffer pieno, i task non vengono consumati abbastanza velocemente.\n";
        std::exit(1);
    }
        if (generate_task_prob(random_engine)) { // Probabilità p di generare un task
            Qualification q = std::uniform_int_distribution<size_t>(1, Q)(random_engine);
            Task new_task{project_id, current_phase, q};
            
            if (!buffer->is_full()) {
                buffer->update(new_task);
                notify(new_task);
            } else {
                std::cerr << "Errore: buffer pieno, simulazione terminata.\n";
                std::exit(1);
            }

            // Determina il prossimo task
            if (current_phase < N) {
                current_phase++;
            } else {
                current_phase = 1;
                project_id++;
            }
        }
    }
};

#endif
