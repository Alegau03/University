#include "wind.hpp"
#include <iostream>
Wind::Wind(std::default_random_engine &random_engine)
            : random_engine(random_engine), random_wind (-1,1){}

void Wind::update(Time time){
    if (time < next_sampling)
        return;
    
    next_sampling = time + T_w;
    W.w_1 = random_wind(random_engine);
    W.w_2 = random_wind(random_engine);
    notify(W);

}
