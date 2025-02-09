#ifndef DELIVERY_MANAGER_HPP
#define DELIVERY_MANAGER_HPP

#include "data.hpp"
#include <iostream>
#include "../mocc/time.hpp"
#include "../mocc/buffer.hpp"
class D_Manager : public Observer<Task,Time>,
                public Observer<Time>,
                public Notifier<Time>
{

    
    public:
        D_Manager(Buffer<Task>* b): b(b){}
        Time completation_time;
        Buffer<Task>* b;
        


        void update(Task task, Time time) override{
        b->update(task);
        completation_time = time;
        notify(time);

    
            }

        void update(Time t) override{

            if (!b->is_empty()){
                Task x = b->pop_front();

            }
        }
};

#endif