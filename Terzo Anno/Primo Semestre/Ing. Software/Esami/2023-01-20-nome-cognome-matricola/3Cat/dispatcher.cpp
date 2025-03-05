#include "dispatcher.hpp"
#include <iostream>
Dispatcher::Dispatcher(std::default_random_engine &random_engine)
                        : random_engine(random_engine), random_input(0,N-1){} 

void Dispatcher::update(Time time){
    if (time < next_request)
        return;
    for (size_t i = 0; i < K; i++){
        notify(i);
    }

}

void Dispatcher::notify(Output output){
    auto inputs = Notifier<Output>::observers;

    if (inputs.empty()) 
        return;
    
    inputs[random_input(random_engine)]->update(output);
}