#include "monitor.hpp"
#include <iostream>

void Monitor::update(Request request){
    a.save(request.a);
    b.save(request.b);
    id.save(request.id);
    interval.save(request.time -last_request);
    last_request = request.time;
}