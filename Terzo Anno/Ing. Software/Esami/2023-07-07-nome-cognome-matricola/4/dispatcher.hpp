#ifndef DSPTCH_HPP_
#define DSPTCH_HPP_

#include "../mocc/buffer.hpp"
#include "../mocc/notifier.hpp"
#include "../mocc/time.hpp"
#include "data.hpp"
#include "worker.hpp" // Per accedere ai worker
#include <random>
#include <vector>
#include <limits>

class Dispatcher : public Notifier<WorkerId, Task>,
                   public Buffer<Job>,
                   public Observer<Time> {
  Time last_pop = 0;
  std::default_random_engine &r_eng;
  std::vector<Worker *> &workers; // Riferimento ai worker

public:
  Dispatcher(size_t limit, std::default_random_engine &r_eng, std::vector<Worker *> &workers)
      : r_eng(r_eng), workers(workers) {
    this->limit = limit;
  }

  void update(Time t) override {
    if (!buffer.empty() && t - last_pop >= D) {
      auto job = buffer.front();
      buffer.pop_front();

      // TROVARE IL MINIMO NUMERO DI TASK TRA I WORKER
      size_t min_tasks = std::numeric_limits<size_t>::max();
      std::vector<Worker *> least_loaded_workers;

      for (auto worker : workers) {
        size_t tasks = worker->getBufferSize();
        if (tasks < min_tasks) {
          min_tasks = tasks;
          least_loaded_workers.clear();
          least_loaded_workers.push_back(worker);
        } else if (tasks == min_tasks) {
          least_loaded_workers.push_back(worker);
        }
      }

      // SELEZIONA RANDOMICAMENTE UNO DEI WORKER MENO OCCUPATI
      if (!least_loaded_workers.empty()) {
        std::uniform_int_distribution<> dist(0, least_loaded_workers.size() - 1);
        Worker *selected_worker = least_loaded_workers[dist(r_eng)];
        notify(selected_worker->getId(), Task{job, 1});
      }

      last_pop = t;
    }
  }

  // Buffer update...
  void update(Job job) override { buffer.push_back(job); }
};

#endif
