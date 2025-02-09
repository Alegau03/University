#ifndef MNTR_HPP_
#define MNTR_HPP_

#include "../mocc/stat.hpp"
#include "data.hpp"
class Monitor : public Observer<InitTime, CompTime> {
public:
  Stat time_stat;
  void update(InitTime init_time, CompTime comp_time) {
    time_stat.save((Time)comp_time - (Time)init_time);
  }
};
#endif