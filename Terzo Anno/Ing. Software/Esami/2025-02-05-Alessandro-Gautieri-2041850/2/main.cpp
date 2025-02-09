#include "drone.hpp"
#include "monitor.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

#define SIMULATIONS 1000

int main() {
    std::random_device r_device;
    std::default_random_engine r_engine(r_device());

    size_t H, N, M;
    real_t X1, X2, Y1, Y2, C;
    std::vector<std::pair<real_t, real_t>> monitor_points;

    std::ifstream params("parameters.txt");
    if (!params) {
        std::cerr << "Errore: impossibile aprire parameters.txt\n";
        return 1;
    }

    std::string word;
    params >> word >> C;  // Soglia C
    params >> word >> H;  // HORIZON
    params >> word >> N;  // Numero di droni
    params >> X1 >> X2 >> Y1 >> Y2;  // Area da monitorare
    params >> word >> M;  // Numero di punti monitorati

    for (size_t i = 0; i < M; i++) {
        real_t x, y;
        params >> x >> y;
        monitor_points.emplace_back(x, y);
    }

    params.close();

    /*
    std::cout << "[INFO] C: " << C << ", H: " << H << ", N: " << N
              << ", Area: [" << X1 << "," << X2 << "]x[" << Y1 << "," << Y2
              << "], M: " << M << std::endl;
    */
    // **Simulazione Monte Carlo**
    size_t successful_simulations = 0;
    for (size_t i = 0; i < SIMULATIONS; i++) {
        std::vector<Drone> drones;
        for (size_t j = 0; j < N; j++) {
            drones.emplace_back(r_engine, X1, X2, Y1, Y2);
        }

        Monitor monitor(monitor_points);

        for (size_t t = 0; t < H; t++) {
            for (auto& d : drones) {
                d.move();
            }
            monitor.update(drones, t);
        }

        if (monitor.coverage_meets_threshold(C)) {
            successful_simulations++;
        }
        /*
        std::cout << "[Simulation] Iteration " << i + 1 
                  << " | Coverage meets C: " << monitor.coverage_meets_threshold(C) 
                  << std::endl;
        */
    }

    real_t probability = static_cast<real_t>(successful_simulations) / SIMULATIONS;


    std::ofstream result("results.txt");
    if (!result) {
        std::cerr << "Errore: impossibile creare results.txt\n";
        return 1;
    }

    result << "2025-02-05-Alessandro-Gautieri-2041850\n";
    result << "P " << probability << "\n";

    result.close();

    //std::cout << "[Finished] Simulation Complete! Results saved in results.txt\n";
    return 0;
}
