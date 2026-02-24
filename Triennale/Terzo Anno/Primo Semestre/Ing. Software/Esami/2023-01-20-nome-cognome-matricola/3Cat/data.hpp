#ifndef DATA_HPP_
#define DATA_HPP_

#include "../mocc/mocc.hpp"
#include "../mocc/alias.hpp"

ALIAS_TYPE(Output, int);


struct Request{
    int a, b, id;
    real_t time;
    Output output;
};

extern real_t T, HORIZON;
extern size_t lambda, N, K; 

#endif