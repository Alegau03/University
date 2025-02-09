#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "data.hpp"
#include "../mocc/time.hpp"
#include "../mocc/recorder.hpp"

class Controller: public Observer<Time>,
                  public Recorder<SetPoint>,
                  public Observer<Speed,Space>,
                  public Notifier<Ctrl>
                  {

        public:
            Space X{0,0,0};
            Speed V{0,0,0};
            void update(Time);
            void update(Speed, Space);
};


#endif