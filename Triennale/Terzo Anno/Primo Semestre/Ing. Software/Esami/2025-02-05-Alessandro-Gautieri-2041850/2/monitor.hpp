#pragma once
#include "../mocc/stat.hpp"
#include <vector>
#include <iostream>
#include <fstream>

class Monitor {
    struct MonitorPoint {
        real_t x, y;
        real_t coverage;
        size_t count;

        MonitorPoint(real_t px, real_t py)
            : x(px), y(py), coverage(0.0), count(0) {}
    };

    std::vector<MonitorPoint> points;

public:
    Monitor(const std::vector<std::pair<real_t, real_t>>& monitor_positions) {
        for (auto& pos : monitor_positions) {
            points.emplace_back(pos.first, pos.second);
        }
    }

    void update(const std::vector<Drone>& drones, size_t t) {
        for (auto& p : points) {
            size_t q = 0;
            for (const auto& d : drones) {
                auto [dx, dy] = d.get_position();
                if (dx >= p.x - 1 && dx <= p.x + 1 &&
                    dy >= p.y - 1 && dy <= p.y + 1) {
                    q++;
                }
            }
            p.coverage = (p.coverage * t + q) / (t + 1);
        }
    }

    // ✅ Metodo per verificare se tutti i punti soddisfano C
    bool coverage_meets_threshold(real_t C) const {
        for (const auto& p : points) {
            if (p.coverage < C) {
                return false;
            }
        }
        return true;
    }
};
