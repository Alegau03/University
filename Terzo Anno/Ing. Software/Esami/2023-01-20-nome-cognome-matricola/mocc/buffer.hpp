#ifndef MOCC_BUFFER_HPP_
#define MOCC_BUFFER_HPP_

#include "observer.hpp"
#include <deque>
#include <exception>

class buffer_full : public std::exception {};

template <typename T>
class Buffer : public Observer<T> {
  protected:
    size_t limit;
    std::deque<T> buffer;

  public:
    Buffer(size_t limit = 0) : limit(limit) {}

    void update(T args) override {
        if (limit > 0 && buffer.size() >= limit) // Fix: Usare >= invece di > per evitare overflow
            throw buffer_full();
        buffer.push_back(args);
    }

    bool is_empty() const { return buffer.empty(); } // Aggiunto metodo

    bool is_full() const { return buffer.size() >= limit; } // Aggiunto metodo

    T pop_front() {
        if (is_empty()) {
            throw std::runtime_error("Tentativo di pop da un buffer vuoto!");
        }
        T item = buffer.front();
        buffer.pop_front();
        return item;
    }
};

#endif
