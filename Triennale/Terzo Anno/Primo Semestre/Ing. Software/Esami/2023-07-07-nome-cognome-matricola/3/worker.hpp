#ifndef WORKER_HPP
#define WORKER_HPP

#include "../mocc/buffer.hpp"
#include "../mocc/notifier.hpp"
#include "../mocc/observer.hpp"
#include "../mocc/time.hpp"
#include "data.hpp"
#include <cmath>
#include <cstdio>
#include <random>
#include <vector>

enum TRANSITION { STAY = 0, NEXT = 1 };
class Worker : public Observer<WorkerId, Task>,
               public Notifier<WorkerId, Task>,
               public Notifier<Task>,
               public Observer<Time>,
               public Buffer<Task> {

  size_t state = 0; // idle
  WorkerId w_id;
  std::default_random_engine &r_eng;
  std::uniform_int_distribution<> r_wrk_id;
  std::vector<std::discrete_distribution<>> trns_mtrx;

public:
  Worker(std::default_random_engine &r_eng, WorkerId w_id)
      : r_eng(r_eng), w_id(w_id), r_wrk_id(1, W) {
    trns_mtrx = std::vector<std::discrete_distribution<>>(N);

    for (int k = 1; k <= N; k++) {
      real_t p_ik = 1. / 2 * exp(-(((real_t)w_id / W) + ((real_t)k / N)));
      trns_mtrx[k - 1] = std::discrete_distribution<>{p_ik, 1 - p_ik};
    }
  }

  void update(WorkerId id, Task task) override {
    if (id == w_id) {
      buffer.push_back(task);
    }
  }
  void update(Time time) override {
    if (state == 0 && (!buffer.empty())) {
      auto nxt_task = buffer.front();
      buffer.pop_front();

      switch (trns_mtrx[nxt_task.phase](r_eng)) {
      case STAY:
        break;
      case NEXT: {
        if (nxt_task.phase + 1 < N) {
          Notifier<WorkerId, Task>::notify(
              r_wrk_id(r_eng), Task{nxt_task.job, nxt_task.phase + 1});
        } else {
          nxt_task.job.comp_time = time;
          Notifier<Task>::notify(Task{nxt_task.job, nxt_task.phase + 1});
        }
        state = 0;
        break;
      }
      }
    }
  }

  real_t getMeanTasks() const {
    printf("buffer.size() = %lu, T = %d\n", buffer.size(), T);
    return (real_t)buffer.size() / (real_t)T;
  }
  real_t getStdDevTasks() const {
    real_t mean = getMeanTasks();
    real_t variance = 0;
    for (size_t i = 0; i < buffer.size(); ++i) {
      auto task = buffer[i];
      variance += pow((real_t)task.phase - mean, 2);
    }
    printf("variance = %f, T = %d\n", variance, T);
    return sqrt(variance / (real_t)T);
  }

};
#endif