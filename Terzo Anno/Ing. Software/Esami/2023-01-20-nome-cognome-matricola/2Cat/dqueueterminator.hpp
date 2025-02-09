#ifndef DQUEUETERMINATOR_HPP
#define DQUEUETERMINATOR_HPP

#include "data.hpp"
#include "../mocc/time.hpp"

class Terminator: public Observer<Time>,
                  public Notifier<Time>{
    public:
        void update(Time);
};

#endif