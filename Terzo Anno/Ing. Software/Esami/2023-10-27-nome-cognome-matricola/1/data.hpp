#ifndef DATA_HPP
#define DATA_HPP

#include "../mocc/alias.hpp"
#include "../mocc/mocc.hpp"

ALIAS_TYPE(Qualification, size_t);

struct Task {
    size_t v, k, q;

};
const real_t T = 1, p = .2;
const size_t W = 5, Q = 5, N = 5, B = 50, HORIZON = 100;

#endif 