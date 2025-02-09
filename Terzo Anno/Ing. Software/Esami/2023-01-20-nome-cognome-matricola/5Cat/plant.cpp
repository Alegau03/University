#include "plant.hpp"
#include <iostream>

void Plant::update(Time time){
    /*
    std::cout << "Ctrl: " << U.u_1 << " " << U.u_2 << " " << U.u_3 << "\n";
    std::cout << "Wind: " << W.w_1 << " " << W.w_2 << "\n";
    std::cout << "Speed: " << V.v_1 << " " << V.v_2 << " " << V.v_3 << "\n";
    std::cout << "Space: " << X.x_1 << " " << X.x_2 << " " << X.x_3 << "\n";
    */ 
        X.x_1 = V.v_1;
        X.x_2 = V.v_2;
        X.x_3 = V.v_3;
        V.v_1 = U.u_1 + W.w_1;
        V.v_2 = U.u_2 + W.w_2;
        V.v_3 = U.u_3 - g;
        notify(V, X); 
}

void Plant::update(Ctrl u){
    U = Ctrl{u.u_1, u.u_2, u.u_3};
}
void Plant::update(Wind_ w){
    W = Wind_{w.w_1, w.w_2};
}