#include "dqueue.hpp"

void DQueue::update(Time time){
    if (!buffer.empty())
        buffer.pop_front();
}