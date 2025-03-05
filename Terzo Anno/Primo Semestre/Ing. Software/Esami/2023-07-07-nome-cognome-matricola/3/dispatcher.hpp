#ifndef DSPTCH_HPP_
#define DSPTCH_HPP_

#include "../mocc/buffer.hpp"
#include "../mocc/notifier.hpp"
#include "../mocc/time.hpp"
#include "data.hpp"
#include <random>

class Dispatcher : public Notifier<WorkerId, Task>,
                   public Buffer<Job>,
                   public Observer<Time> {

  Time last_pop = 0;
  std::default_random_engine &r_eng;
  std::uniform_int_distribution<> wrk_id;

public:
  Dispatcher(size_t limit, std::default_random_engine &r_eng)
      : r_eng(r_eng), wrk_id(1, W) {
    this->limit = limit;
  }
  void update(Time t) override {
    if (!buffer.empty() && t - last_pop >= D) {
      auto job = buffer.front();
      buffer.pop_front();
      notify(wrk_id(r_eng), Task{job, 1});
      last_pop = t;
    }
  }
  // Buffer update...
  void update(Job job) override { buffer.push_back(job); } 
};

#endif