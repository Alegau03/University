#ifndef WIND_HPP
#define WIND_HPP

#include "data.hpp"
#include "../mocc/time.hpp"

class Wind : public Observer<Time>,
             public Notifier<Wind_>{
    std::default_random_engine &random_engine;
    public:

        Wind_ W{0,0};
        real_t next_sampling = 0;
        std::uniform_real_distribution<real_t> random_wind;
        Wind(std::default_random_engine &random_engine);
        void update(Time);

};

#endif