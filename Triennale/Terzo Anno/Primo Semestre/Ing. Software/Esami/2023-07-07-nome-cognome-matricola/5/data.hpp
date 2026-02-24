#ifndef DATA_HPP_
#define DATA_HPP_

#include "../mocc/alias.hpp"
#include "../mocc/mocc.hpp"
#include "../mocc/time.hpp"
#include <cstddef>

static int W = 5, N = 10, B = 20, L = 10, D = 1, T = 1;

struct Job {
  Time init_time;
  Time comp_time;
  size_t job_id;
};

ALIAS_TYPE(InitTime, Time);
ALIAS_TYPE(CompTime, Time)
ALIAS_TYPE(WorkerId, size_t);

struct Task {
  Job job;
  size_t phase;
};

#endif