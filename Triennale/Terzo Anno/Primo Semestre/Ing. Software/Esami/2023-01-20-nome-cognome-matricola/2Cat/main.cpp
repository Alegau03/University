#include "main.hpp"

int main(){
    std::random_device random_device;
    size_t my_seed = random_device();
    std::default_random_engine random_engine(my_seed);
    Stopwatch timer(T);
    Monitor monitor;
    Env env(random_engine);
    DQueue dqueue(10);
    Terminator terminator;
    timer.addObserver(&env);
    timer.addObserver(&terminator);
    env.addObserver(&monitor);
    env.addObserver(&dqueue);
    terminator.addObserver(&dqueue);
    while (timer.elapsed() < HORIZON){
        timer.tick();
    }

    std::ofstream Output("outputs.txt");
    Output << "2023-01-20-Catalin-Ghitun-2059802\n"
    << monitor.a.mean() << " " << monitor.a.stddev_welford() << "\n"
    << monitor.b.mean() << " " << monitor.b.stddev_welford() << "\n" 
    << monitor.id.mean() << " " << monitor.id.stddev_welford() << "\n"
    << monitor.interval.mean() << " " << monitor.interval.stddev_welford() << "\n";

}