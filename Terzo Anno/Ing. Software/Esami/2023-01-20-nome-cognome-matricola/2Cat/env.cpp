#include "env.hpp"
#include <iostream>
Env::Env(std::default_random_engine &random_engine):
        random_engine(random_engine), random_int(-1000,1000), random_interval(10){
            next_request = random_interval(random_engine);
        }

void Env::update(Time time){
    if (time < next_request)
        return;
    Request request{random_int(random_engine), random_int(random_engine), random_int(random_engine), time};
    notify(request);
    next_request = time + random_interval(random_engine); 
}