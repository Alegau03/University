#ifndef DQUEUE_HPP
#define DQUEUE_HPP

#include "data.hpp"
#include "../mocc/time.hpp"
#include "../mocc/buffer.hpp"

class DQueue : public Buffer<Request>,
               public Observer<Time>
            {
                
        public:
             DQueue(size_t limit) : Buffer(limit){};
             void update(Time);
};


#endif