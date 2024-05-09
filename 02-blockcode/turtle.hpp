#pragma once
#include "vec.hpp"
struct Turtle {
  Vecf pos_{};
  Vecf dir_{};
  float one_step_length_{0.0f};
};

Turtle defaul_turtle(float one_step_length) {
  return Turtle{make_vec(0.5f, 0.5f), make_vec(1.0f, 0.0f), one_step_length};
}