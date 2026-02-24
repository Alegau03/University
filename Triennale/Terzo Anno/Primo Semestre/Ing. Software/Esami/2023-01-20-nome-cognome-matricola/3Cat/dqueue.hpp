#ifndef DQUEUE_HPP
#define DQUEUE_HPP

#include "data.hpp"
#include "../mocc/time.hpp"
#include "../mocc/buffer.hpp"

class DQueue : public Buffer<Request>,
               public Observer<Output>,
               public Notifier<Request>
            {
                
        public:
            size_t id;
            DQueue(size_t limit, size_t id);
            void update(Output) override;
            void notify(Request) override; 
};


#endif