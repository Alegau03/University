#ifndef MONITOR_HPP
#define MONITOR_HPP

#include "data.hpp"
#include "../mocc/time.hpp"
#include "../mocc/recorder.hpp"
#include "../mocc/stat.hpp"
class Monitor : public Recorder<SetPoint>,
                public Observer<Speed, Space>,
                public Observer<Time>{

    public:
        Stat err_1;
        Stat err_2;
        Stat err_3;
        Space X{0,0,0};
        void update(Time);
        void update(Speed, Space);
};

#endif