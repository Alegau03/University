#ifndef MONITOR_HPP
#define MONITOR_HPP

#include "data.hpp"
#include "../mocc/stat.hpp"
#include "../mocc/time.hpp"

class Monitor : public Observer<Request>{
    public:
        Stat a,b, id, interval;
        Time last_request = 0;
        void update(Request);
};

#endif