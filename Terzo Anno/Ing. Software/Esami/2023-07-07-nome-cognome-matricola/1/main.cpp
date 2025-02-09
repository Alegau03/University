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

  for (int i = 1; i <= W; i++) {
    Employee *emp = new Employee(r_eng, i);
    timer.addObserver(emp);
    emp->addObserver(&monitor);
    employees.push_back(emp);
  }

  while (timer.elapsed() < HORIZON) {
    bool early_stop = timer.elapsed() > 1000;
    for (auto emp : employees) {
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
  result << "Dipendente AvgTime AvgCost  StdDevTime StdDevCost (ID = 2058986, "
            "time=10000)\n";
  result << monitor.time_stat.mean() << " " << monitor.cost_stat.mean() << " "
         << monitor.time_stat.stddev_welford() << " "
         << monitor.cost_stat.stddev_welford() << "\n";
  for (auto emp : employees) {
    result << emp->time_stat.mean() << " " << emp->time_stat.mean() * emp->cost
           << " " << emp->time_stat.stddev_welford() << " "
           << emp->time_stat.stddev_welford() * emp->cost << "\n";
  }
  result.close();
}