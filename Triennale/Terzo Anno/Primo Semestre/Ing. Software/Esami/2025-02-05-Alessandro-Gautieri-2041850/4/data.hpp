#ifndef DATA_HPP
#define DATA_HPP

#include "../mocc/alias.hpp"
#include "../mocc/time.hpp"
#include "../mocc/stat.hpp"

static int T = 1;  // Unità di tempo

enum RequestType { IDLE = 0, REQUEST_1 = 1, REQUEST_2 = 2 };

struct Request {
    Time init_time;
    size_t id;
    RequestType type;
};

#endif
