#ifndef MQUEUE_HPP
#define MQUEUE_HPP

#include "data.hpp"
#include "../mocc/time.hpp"
#include "../mocc/buffer.hpp"

class MQueue : public Buffer<Request>,
               public Notifier<Request>{
    public:
        size_t id;
        MQueue(size_t k, size_t limit);
        void update(Request) override;
};

#endif