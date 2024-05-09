#pragma once
#include "vec.hpp"
struct Turtle {
  Vecf pos_{};
  Vecf dir_{};
  float one_step_length_{0.0f};
};

Turtle defaul_turtle(const Vecf &start, float one_step_length) {
  return Turtle{start, make_vec(0.0f, 1.0f), one_step_length};
}