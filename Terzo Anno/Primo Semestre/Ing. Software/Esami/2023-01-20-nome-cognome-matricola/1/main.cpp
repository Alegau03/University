#include "data.hpp"
#include "employee.hpp"
#include "monitor.hpp"
#include <fstream>
#include <random>
#include <vector>

#define HORIZON 10000

int main() {
  std::random_device r_dev;
  std::default_random_engine r_eng(r_dev());

  Stopwatch timer(T);
  Monitor monitor;
  std::vector<Employee *> employees;

  // Ciclo per creare i dipendenti, assegnato ad ogni dipendente un id e aggiungendolo alla lista, inoltre viene aggiunto l'osservatore monitor 
  for (int i = 1; i <= W; i++) {
    Employee *emp = new Employee(r_eng, i);
    timer.addObserver(emp);
    emp->addObserver(&monitor);
    employees.push_back(emp);
  }
  // Ciclo per simulare il lavoro dei dipendenti
  while (timer.elapsed() < HORIZON) {
    bool early_stop = timer.elapsed() > 1000;
    for (auto emp : employees) {
      // Questo if serve per fermare la simulazione se la deviazione standard è minore del 10% della media
      if (emp->time_stat.stddev_welford() > 0.1 * emp->time_stat.mean()) {
        early_stop = false;
        break;
      }
    }
    if (early_stop)
      break;
    timer.tick();
  }
  std::ofstream result("result.txt");
  // Scrittura della seconda riga con i parametri richiesti
    result << "A = " << A << ", B = " << B << ", C = " << C << ", D = " << D
           << ", F = " << F << ", G = " << G << ", N = " << N << ", W = " << W
           << ", AvgTime = " << monitor.time_stat.mean()
           << ", AvgCosto = " << monitor.cost_stat.mean() << "\n";
    
    // Scrittura delle righe per ogni dipendente
    for (auto emp : employees) {
        result << emp->getiD() << " "
               << emp->time_stat.mean() << " "
               << emp->time_stat.mean() * emp->cost << " "
               << emp->time_stat.stddev_welford() << " "
               << emp->time_stat.stddev_welford() * emp->cost << "\n";
    }
    
    result.close();
    return 0;
}