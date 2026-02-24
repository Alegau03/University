#include "monitor.hpp"
#include <iostream>
void Monitor::update(Time time){
    SetPoint R = Recorder<SetPoint>::record;
    err_1.save(X.x_1-R.r_1);
    err_2.save(X.x_2-R.r_2);
    err_3.save(X.x_3-R.r_3);
    std::cout << "Space: " << X.x_1 << " " << X.x_2 << " " << X.x_3 << "\n";


}

void Monitor::update(Speed v, Space x){
    X = Space{x.x_1,x.x_2,x.x_3};
}