#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "data.hpp"
#include "../mocc/time.hpp"

class Dispatcher : public Observer<Time>,
                   public Notifier<Output>{
    std::default_random_engine &random_engine;
    public:
        Time next_request = 5*T;
        std::uniform_int_distribution<> random_input;
        Dispatcher(std::default_random_engine&);
        void update(Time) override;
        void notify(Output) override;
};

#endif