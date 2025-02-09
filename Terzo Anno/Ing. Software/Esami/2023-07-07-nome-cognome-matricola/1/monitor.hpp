#ifndef MONITOR_HPP_
#define MONITOR_HPP_
#include "../mocc/time.hpp"

#include "../mocc/stat.hpp"
class Monitor : public Observer<Time, real_t> {

public:
  Stat time_stat;
  Stat cost_stat;
  void update(Time comp_time, real_t cost) {
    time_stat.save(comp_time);
    cost_stat.save(cost * comp_time);
  }
};
#endif