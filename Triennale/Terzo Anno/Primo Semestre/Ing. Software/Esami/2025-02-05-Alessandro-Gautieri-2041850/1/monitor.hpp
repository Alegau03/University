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
    // Costruttore che inizializza i punti monitorati
    Monitor(const std::vector<std::pair<real_t, real_t>>& monitor_positions) {
        for (auto& pos : monitor_positions) {
            points.emplace_back(pos.first, pos.second);
        }
    }

    //  Metodo per aggiornare la copertura in ogni timestep
    void update(const std::vector<Drone>& drones, size_t t) {
        for (auto& p : points) {
            size_t q = 0;
            for (const auto& d : drones) {
                auto [dx, dy] = d.get_position();
                if (dx >= p.x - 1 && dx <= p.x + 1 &&
                    dy >= p.y - 1 && dy <= p.y + 1) {
                    q++;  // Il drone è nell'area di monitoraggio
                }
            }
            // Aggiornamento della copertura secondo la formula richiesta
            p.coverage = (p.coverage * t + q) / (t + 1);
        }
    }

    //  Metodo per salvare i risultati, li stampo da qui per comodità, dal main chiamerò questo metodo
    void save_results(const std::string& filename) {
        std::ofstream result(filename);
        if (!result) {
            std::cerr << "Errore: impossibile creare results.txt\n";
            return;
        }

   
        result << "2025-02-05-Alessandro-Gautieri-2041850\n";
        for (const auto& p : points) {
            result << p.x << " " << p.y << " " << p.coverage << "\n";
        }
        result.close();
    }
};
