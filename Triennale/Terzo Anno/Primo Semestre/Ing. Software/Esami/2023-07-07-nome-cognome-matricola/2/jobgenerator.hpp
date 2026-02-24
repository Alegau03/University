#ifndef JBGEN_HPP_
#define JBGEN_HPP_

#include "../mocc/notifier.hpp"
#include "../mocc/time.hpp"
#include "data.hpp"
#include <cstdio>

enum STATE { IDLE = 0, SEND = 1 };

class JobGenerator : public Notifier<Job>, public Observer<Time> {
  STATE state = IDLE;
  size_t jobcounter = 0;
  std::default_random_engine &r_eng;
  std::vector<std::discrete_distribution<>> trns_mtrx;

public:
  JobGenerator(std::default_random_engine &r_eng) : r_eng(r_eng) {
    real_t p = 1 - (1. / L);
    trns_mtrx = std::vector<std::discrete_distribution<>>(2);
    trns_mtrx[0] = std::discrete_distribution<>{p, 1 - p};
    trns_mtrx[1] = {1};
  }

  void update(Time time) override {
    state = (STATE)trns_mtrx[static_cast<size_t>(state)](r_eng);
    switch (state) {
    case IDLE:
      break;
    case SEND: {
      notify((Job){time, jobcounter++});
    }
    }
  }
};

#endif