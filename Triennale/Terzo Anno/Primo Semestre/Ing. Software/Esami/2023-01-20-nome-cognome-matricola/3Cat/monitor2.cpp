#include "monitor2.hpp"

Monitor::Monitor(size_t k): id(k){}

void Monitor::update(Request request){
    number_of_request++;
}
