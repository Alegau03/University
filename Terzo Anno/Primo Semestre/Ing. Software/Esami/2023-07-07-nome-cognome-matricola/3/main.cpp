#include "data.hpp"
#include "deliverymanager.hpp"
#include "dispatcher.hpp"
#include "jobgenerator.hpp"
#include "monitor.hpp"
#include "worker.hpp"
#include <fstream>
#include <random>
#include <vector>
#define HORIZON 10000

int main() {
    std::random_device r_dev;
    std::default_random_engine r_eng(r_dev());

    Monitor monitor;
    Stopwatch timer(T);
    Dispatcher dispatcher(B, r_eng);
    DeliveryManager delMan(B);
    JobGenerator jobGen(r_eng);
    std::vector<Worker *> workers;

    jobGen.addObserver(&dispatcher);
    timer.addObserver(&jobGen);
    timer.addObserver(&dispatcher);
    timer.addObserver(&delMan);
    delMan.addObserver(&monitor);

    for (int i = 1; i <= W; i++) {
        Worker *worker = new Worker(r_eng, i);
        timer.addObserver(worker);
        dispatcher.addObserver(worker);
        worker->Notifier<Task>::addObserver(&delMan);
        workers.push_back(worker);
    }

    for (auto worker : workers) {
        for (auto worker_ : workers) {
            worker->Notifier<WorkerId, Task>::addObserver(worker_);
        }
    }

    // Simulazione con orizzonte di 10000 giorni
    while (timer.elapsed() < HORIZON) {
        timer.tick();
    }

    // Scrittura dei risultati in outputs.txt
    std::ofstream result("outputs.txt");
    result << "ID= yyy, MyMagicNumber= zzz, time=" << timer.elapsed() << "\n";
    
    for (int i = 0; i < workers.size(); i++) {
        result << "i= " << (i + 1) << ", b= " << workers[i]->getMeanTasks() << ", s= " << workers[i]->getStdDevTasks() << "\n";
    }
    result.close();

    return 0;
}
