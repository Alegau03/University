#ifndef MNTR_HPP_
#define MNTR_HPP_

#include "../mocc/stat.hpp"
#include "../mocc/time.hpp"
#include "data.hpp"
class Monitor : public Observer<Time> {
real_t last_request_time = 0;

  public:
    Stat interval;
    void update(Time time) override{
    interval.save(time - last_request_time);
    last_request_time = time;
}
};
#endif