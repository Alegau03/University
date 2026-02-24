#pragma once
#include "../mocc/time.hpp"
#include <random>
#include <iostream>

class Drone {
    real_t x, y;  // Posizione attuale del drone
    real_t X1, X2, Y1, Y2;  // Limiti dell'area
    std::default_random_engine &r_engine;
    std::uniform_real_distribution<real_t> velocity{-0.5, 0.5};  // Distribuzione per vx, vy

public:
    Drone(std::default_random_engine &engine, real_t x1, real_t x2, real_t y1, real_t y2)
        : r_engine(engine), X1(x1), X2(x2), Y1(y1), Y2(y2) {
        // Inizializza la posizione casuale all'interno dell'area
        std::uniform_real_distribution<real_t> x_dist(X1, X2);
        std::uniform_real_distribution<real_t> y_dist(Y1, Y2);
        x = x_dist(r_engine);
        y = y_dist(r_engine);
    }
    // Muove il drone in una posizione casuale all'interno dell'area
    void move() {
        real_t vx = velocity(r_engine);
        real_t vy = velocity(r_engine);

        x = std::max(X1, std::min(X2, x + vx));
        y = std::max(Y1, std::min(Y2, y + vy));
    }
    // Restituisce la posizione attuale del drone, dopo averlo mosso
    std::pair<real_t, real_t> get_position() const {
        return {x, y};
    }
};
