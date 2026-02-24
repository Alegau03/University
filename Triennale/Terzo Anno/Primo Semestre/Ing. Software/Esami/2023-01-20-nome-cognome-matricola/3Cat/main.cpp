#include "main.hpp"

int main(){
    std::random_device random_device;
    size_t my_seed = random_device();
    std::default_random_engine random_engine(my_seed);
    Stopwatch timer(T);
    Dispatcher dispatcher(random_engine);
    Env env(random_engine);
    std::vector<DQueue*> inputs;
    std::vector<MQueue*> outputs;
    std::vector<Monitor*> monitors;


    for (size_t i = 0; i < N; i++){
        DQueue* dqueue = new DQueue(1000,i);
        env.addObserver(dqueue);
        inputs.push_back(dqueue);
        dispatcher.addObserver(dqueue);

    }

    for (size_t i = 0; i < K; i++){
        Monitor* monitor = new Monitor(i);
        monitors.push_back(monitor);
        MQueue* mqueue = new MQueue(1000,i);
        mqueue->addObserver(monitor);
        outputs.push_back(mqueue);
        for (DQueue* &dqueue : inputs){
            dqueue->addObserver(mqueue);
        }
    }
    

    timer.addObserver(&env);
    timer.addObserver(&dispatcher);
    while (timer.elapsed() < HORIZON){
        timer.tick();
    }

    std::ofstream Output("outputs.txt");
    Output << "2023-01-20-Catalin-Ghitun-2059802\n";
    Output << "A";
    for (Monitor* &monitor : monitors){
        Output << monitor->id << " " << monitor->number_of_request/HORIZON << "\n" << "A"; 
    }


}