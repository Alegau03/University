#include "main.hpp"

int main(){
    std::random_device random_device;
    size_t myseed = random_device();
    std::default_random_engine random_engine(myseed);
    Wind wind(random_engine);
    User user;
    Controller controller;
    Plant plant;
    Monitor monitor;
    Stopwatch timer(T);
    timer.addObserver(&wind);
    wind.addObserver(&plant);
    timer.addObserver(&controller);
    timer.addObserver(&monitor);
    controller.addObserver(&plant);
    timer.addObserver(&plant);
    plant.addObserver(&controller);
    user.addObserver(&controller);
    user.addObserver(&monitor);
    plant.addObserver(&monitor);

    SetPoint r{10,20,30};
    user.notify(r); 

    while (timer.elapsed() < HORIZON){
        timer.tick();
    }

    std::ofstream Output("outputs.txt");
    Output << "2023-01-20-Catalin.Ghitun" << "\n" <<
    "1 " << monitor.err_1.mean() << " "  << monitor.err_1.stddev_welford() << "\n"
    "2 " << monitor.err_2.mean() << " "  << monitor.err_2.stddev_welford() << "\n"
    "3 " << monitor.err_3.mean() << " "  << monitor.err_3.stddev_welford() << "\n";



    return 0;
}