#ifndef DATA_HPP
#define DATA_HPP

#include "../mocc/mocc.hpp"

struct Request{
    int a, b, id;
    real_t time;
};

extern real_t T, HORIZON;
extern size_t lambda; 

#endif