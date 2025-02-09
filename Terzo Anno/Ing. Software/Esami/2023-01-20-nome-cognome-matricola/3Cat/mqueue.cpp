#include "mqueue.hpp"
#include <iostream>

MQueue::MQueue(size_t limit, size_t k)
               : Buffer<Request>(limit), id(k) {}

void MQueue::update(Request request){
        if (limit > 0 && buffer.size() > limit)
            throw buffer_full();
        if (id == 1){
                std::cout << "A";
        }
        buffer.push_back(request);
        notify(request);
}