#ifndef MONITOR2_HPP
#define MONITOR2_HPP

#include "data.hpp"
#include "../mocc/time.hpp"
#include "../mocc/stat.hpp"

class Monitor : public Observer<Request>{

    public:
        size_t number_of_request, id;
        Monitor(size_t);
        void update(Request);
};

#endif