#include "drone.hpp"
#include "monitor.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

int main() {
    std::random_device r_device;
    std::default_random_engine r_engine(r_device());

    size_t H, N, M;
    real_t X1, X2, Y1, Y2;
    std::vector<std::pair<real_t, real_t>> monitor_points;


    std::ifstream params("parameters.txt");
    if (!params) {
        std::cerr << "Errore: impossibile aprire parameters.txt\n";
        return 1;
    }

    std::string word;
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
    std::cout << "[INFO] H: " << H << ", N: " << N << ", Area: [" 
              << X1 << "," << X2 << "]x[" << Y1 << "," << Y2 << "], M: " << M << std::endl;
    */
    // **Creazione dei droni**
    std::vector<Drone> drones;
    for (size_t i = 0; i < N; i++) {
        drones.emplace_back(r_engine, X1, X2, Y1, Y2);
    }

    // **Creazione del monitor**
    Monitor monitor(monitor_points);

    // **Simulazione**
    for (size_t t = 0; t < H; t++) {
        for (auto& d : drones) {
            d.move();
        }
        monitor.update(drones, t);
    }

    // **Salvataggio dei risultati, da monitor
    monitor.save_results("results.txt");

    //std::cout << "[finito] Simulation Complete! Results saved in results.txt\n";
    return 0;
}
