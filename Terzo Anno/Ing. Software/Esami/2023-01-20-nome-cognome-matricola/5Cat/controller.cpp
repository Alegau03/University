#include "controller.hpp"
#include <iostream>

void Controller::update(Time time){
    SetPoint R = Recorder<SetPoint>::record;
    real_t u_1 = k_1*(X.x_1-R.r_1)+k_2*V.v_1;
    real_t u_2 = k_1*(X.x_2-R.r_2) + k_2*V.v_2;
    real_t u_3 = g + k_1*(X.x_3-R.r_3) + k_2*V.v_3; 
    Ctrl u{u_1,u_2,u_3};
    notify(u);
}

void Controller::update(Speed v, Space x){
    V = Speed{v.v_1, v.v_2, v.v_3};
    X = Space{x.x_1, x.x_2, x.x_3};
}