#ifndef PLANT_HPP
#define PLANT_HPP

#include "data.hpp"
#include "../mocc/time.hpp"


class Plant : public Observer<Time>,
              public Observer<Ctrl>,
              public Observer<Wind_>,
              public Notifier<Speed, Space>
{

    public:
        Speed V{0,0,0};
        Space X{0,0,0};
        Ctrl U{0,0,0};
        Wind_ W{0,0};
        void update(Time);
        void update(Ctrl);
        void update(Wind_);
};


#endif