#include <random>
#include <iostream>
#include <fstream>
#include "data.hpp"
#include "monitor.hpp"
#include "employee.hpp"
#include "project_manager.hpp"
#include "delivery_manager.hpp"
#include "task_generator.hpp"
#include "../mocc/buffer.hpp"
#include "../mocc/time.hpp"


#define HORIZON 10000
int main() {
    std::random_device MyRandomDevice;
    size_t MySeed = MyRandomDevice();
    std::default_random_engine random_engine(MySeed);
    Monitor *m = new Monitor();
    Buffer<Task> *b = new Buffer<Task>(B);
    Buffer<Task> *b_2 = new Buffer<Task>(B);
    TaskGenerator *generator = new TaskGenerator(random_engine, b);
    Stopwatch timer(T);
    std::vector<std::vector<Employee*>> employees(Q);
    P_Manager *manager = new P_Manager(random_engine, b, employees); 
    D_Manager *man = new D_Manager(b_2);

    for (size_t k = 1; k <= W; k++){
        for (size_t q=1; q <= Q; q++){
        Employee *e = new Employee(random_engine, q,k);
        manager->addObserver(e);
        timer.addObserver(e);
        e->addObserver(man);
        employees[q-1].push_back(e);

        }
    }
    man->addObserver(m);
    timer.addObserver(generator);
    timer.addObserver(manager);
    timer.addObserver(man);
    generator->addObserver(b);


  while (timer.elapsed() < HORIZON) {
    timer.tick();
  }
  std::ofstream result("result.txt");

  result <<"V = " << m->interval.mean() << " " << "S = "<< m->interval.stddev_welford() << "\n";
  for (size_t q = 1; q <= Q; q++) {
        for (size_t i = 1; i <= W; i++) {
            Employee* e = employees[q - 1][i - 1];
            result << "i=" << i << ", q=" << q
                   << ", b=" << e->mean_working_percentage()
                   << ", s=" << e->stddev_working_percentage() << "\n";
        }
    }
  result.close();
}