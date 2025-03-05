#ifndef DLVRMAN_HPP_
#define DLVRMAN_HPP_
#include "../mocc/buffer.hpp"
#include "../mocc/time.hpp"
#include "data.hpp"
#include <cstddef>
class DeliveryManager : public Observer<Task>,
                        public Buffer<Job>,
                        public Observer<Time>,
                        public Notifier<InitTime, CompTime> {

  Time last_pop = 0;

public:
  DeliveryManager(size_t limit) { this->limit = limit; }
  void update(Task task) override {
    if (task.phase >= N) {
      notify(task.job.init_time, task.job.comp_time);
      Buffer<Job>::update(task.job);
    }
  };
  void update(Time time) override {
    if (time - last_pop >= D && (!buffer.empty())) {
      buffer.pop_front();
      last_pop = time;
    }
  };
};

#endif