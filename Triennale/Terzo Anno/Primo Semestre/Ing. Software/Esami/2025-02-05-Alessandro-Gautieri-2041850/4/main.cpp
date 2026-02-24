#include "customer.hpp"
#include "server.hpp"
#include "data.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

#define HORIZON 10000
#define SIMULATIONS 1000

int main() {
    std::random_device r_device;
    std::default_random_engine r_engine(r_device());

    Stopwatch timer(T);
    std::vector<std::vector<double>> transition_matrix(3, std::vector<double>(3));
    real_t T1 = 0, T2 = 0;

    // Lettura del file parameters.txt
    std::ifstream params("parameters.txt");
    if (!params) {
        std::cerr << "Errore: impossibile aprire parameters.txt\n";
        return 1;
    }

    int from, to;
    double probability;
    std::vector<double> last_values;
    std::string line;

    while (std::getline(params, line)) {
        std::istringstream iss(line);
        if (iss >> from >> to >> probability) {
            if (from < 3 && to < 3) {
                transition_matrix[from][to] = probability;
            }
        } else {
            double value;
            std::istringstream iss_value(line);
            if (iss_value >> value) {
                last_values.push_back(value);
            }
        }
    }

    params.close();

    if (last_values.size() >= 2) {
        T1 = last_values[last_values.size() - 2];
        T2 = last_values[last_values.size() - 1];
    } else {
        std::cerr << "Errore: T1 e T2 non trovati nel file!" << std::endl;
        return 1;
    }

    //std::cout << "[INFO] T1: " << T1 << ", T2: " << T2 << std::endl;

    // Simulazione Monte Carlo
    Stat stat;
    for (int i = 0; i < SIMULATIONS; i++) {
        Customer customer(r_engine, transition_matrix);
        Server server(T1, T2);
        timer.reset();

        timer.addObserver(&customer);
        customer.addObserver(&server);

        while (timer.elapsed() < HORIZON) {
            timer.tick();
            server.process(timer.elapsed());
            
        }

        real_t avg_queue_length = server.get_average_queue_length();
        stat.save(avg_queue_length);

        //std::cout << "[Simulation] Iteration " << i + 1 << " Avg Queue Length: " << avg_queue_length << std::endl;

        customer.reset();
        server.reset();  // Resetto il server per la prossima simulazione
    }


    std::ofstream result("results.txt");
    if (!result) {
        std::cerr << "Errore: impossibile creare results.txt\n";
        return 1;
    }

    result << "2025-02-05-Alessandro-Gautieri-2041850\n";
    result << "Avg " << stat.mean() << "\n";

    result.close();

   // std::cout << "[Finuto] Simulation Complete! Results saved in results.txt\n";

    return 0;
}
