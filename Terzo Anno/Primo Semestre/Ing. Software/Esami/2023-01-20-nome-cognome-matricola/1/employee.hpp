#ifndef EMPLOYEE_HPP_
#define EMPLOYEE_HPP_

#include "../mocc/stat.hpp"
#include "../mocc/observer.hpp"
#include "../mocc/notifier.hpp"
#include "../mocc/mocc.hpp"
#include "../mocc/time.hpp"
#include "data.hpp"
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>

class Employee : public Observer<Time>, public Notifier<Time, real_t> {

  size_t id;
  size_t phase = 0;
  std::vector<std::discrete_distribution<>> transition_matrix;
  std::default_random_engine &r_eng;
  Time init_time = 0;

public:
  real_t cost;
  Stat time_stat;
  size_t getiD() { return id; }
  // Costruttore della classe Employee 
  Employee(std::default_random_engine &r_eng, size_t id)
      : r_eng(r_eng), id(id), cost(1000 - 500 * (real_t)(id - 1) / (W - 1)) {
    transition_matrix = std::vector<std::discrete_distribution<>>(N);
    real_t tau, theta, alpha, p_stay, p_forward;
    alpha = 1. / (F * (G * W - id));
    tau = (A + B * (id*id) + C * 1 + D * id * 1);
    theta = tau / T;
    p_stay = 1 - (1 / theta);
    p_forward = 1 - p_stay;
    transition_matrix[0] = std::discrete_distribution<>{p_stay, p_forward}; // Questo assegnamento serve per la prima riga della matrice di transizione
    // Ciclo per creare la matrice di transizione per ogni dipendente 
    for (int i = 2, i_pos = 1; i < N; i++, i_pos++) {
      theta = (A + B * (id*id) + C * (i*i) + D * id * i);
      p_stay = 1 - (1 / theta);
      p_forward = (1 - alpha) * (1 - p_stay);
      std::vector<real_t> row(N, .0);
      row[i_pos] = p_stay;        // p_ii
      row[i_pos + 1] = p_forward; // p_ii+1
      // Ciclo per creare la matrice di transizione per ogni dipendente
      for (int j = 0; j < i_pos; j++) {
        row[j] = alpha * ((1 - p_stay) / (i - 1)); // tornare indietro
      }
      transition_matrix[i_pos] =
          std::discrete_distribution<>(row.begin(), row.end());
    }
    transition_matrix[N - 1] = std::discrete_distribution<>{1};
  }

  // Funzione per aggiornare il tempo di lavoro di ogni dipendente
  void update(Time t) override {
    phase = transition_matrix[phase](r_eng);
    if (phase == N - 1) {
      notify(t - init_time, cost); // Notifica il monitor 
      time_stat.save(t - init_time);
      init_time = t;
    }
  }
};

#endif