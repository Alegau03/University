#ifndef DATA_HPP
#define DATA_HPP

#include "../mocc/mocc.hpp"

struct SetPoint{
    real_t r_1, r_2, r_3;
};
struct Ctrl{
    real_t u_1, u_2, u_3;
};
struct Speed{
    real_t v_1,v_2,v_3;
};
struct Space{
    real_t x_1, x_2, x_3;
};
struct Wind_{
    real_t w_1, w_2;
};

extern real_t T, HORIZON, T_w, p, k_1, k_2, g;
real_t T_w(0.1), T(0.001), p(-1), k_1(-(p*p)), k_2(2*p), HORIZON(0.010), g(9.81);
#endif