#ifndef ENV_HPP
#define ENV_HPP

#include "data.hpp"
#include "../mocc/time.hpp"

class Env : public Observer<Time>, 
            public Notifier<Request>{
    std::default_random_engine &random_engine;
    std::uniform_int_distribution<> random_int;
    std::poisson_distribution<> random_interval;
    public:
        real_t next_request = 0;
        Env(std::default_random_engine&);
        void update(Time);
};

#endif