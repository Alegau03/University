#include "dqueue.hpp"
#include <iostream>
DQueue::DQueue(size_t limit, size_t k)
            : Buffer<Request>(limit), id(k){
            }

void DQueue::update(Output output){

    if (!buffer.empty()){
        Request request = buffer.front();
        request.output = output;
        notify(request);
        buffer.pop_front();
    }
}

void DQueue::notify(Request request){
    auto outputs = Notifier<Request>::observers;
    if (outputs.empty())
        return;
    outputs[request.output]->update(request);
}