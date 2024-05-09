#pragma once
#include <cmath>
#include <memory>
#include <vector>

#include "draw_line.hpp"
#include "turtle.hpp"
struct Block {
  virtual ~Block() {}
  virtual void drive(Turtle& turtle) = 0;
};

struct Repeat : public Block {
  Repeat(uint32_t times) : times_{times} {}
  virtual ~Repeat() {}
  void drive(Turtle& turtle) override {
    for (uint32_t i{0}; i < times_; i++) {
      for (auto& block : subBlocks_) {
        block->drive(turtle);
      }
    }
  }
  uint32_t times_{0};
  std::vector<std::unique_ptr<Block>> subBlocks_;
};

template <typename... T>
std::unique_ptr<Block> repeat(uint32_t times, T&&... args) {
  auto res = std::make_unique<Repeat>(times);
  (res->subBlocks_.push_back(std::move(args)),...);
  return res;
}

struct ForwardSteps : public Block {
  ForwardSteps(float step) : step_{step} {}
  virtual ~ForwardSteps() {}
  void drive(Turtle& turtle) override {
    auto new_pos =
        turtle.pos_ + turtle.dir_ * (turtle.one_step_length_ * step_);
    draw_line(turtle.pos_, new_pos);
    turtle.pos_ = new_pos;
  }
  float step_{0.0f};
};
std::unique_ptr<Block> forward_steps(float step) {
  auto res = std::make_unique<ForwardSteps>(step);
  return res;
}
#define MY_PI 3.14159265358979323846f
Vecf rotate_ccw(const Vecf& dir, float radian) {
  Vecf res;
  float sin_res = std::sin(radian);
  float cos_res = std::cos(radian);
  res.x_ = cos_res * dir.x_ - sin_res * dir.y_;
  res.y_ = sin_res * dir.x_ + cos_res * dir.y_;
  return res;
}
struct RightDegrees : public Block {
  RightDegrees(float degree) : degree_{degree / 180.f * MY_PI} {}
  virtual ~RightDegrees() {}
  void drive(Turtle& turtle) override {
    turtle.dir_ = rotate_ccw(turtle.dir_, -degree_);
  }
  float degree_{0.0f};
};
std::unique_ptr<Block> right_degrees(float degree) {
  auto res = std::make_unique<RightDegrees>(degree);
  return res;
}