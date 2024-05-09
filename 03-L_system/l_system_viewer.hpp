#pragma once
#include "block.hpp"
#include "l_system.hpp"
#include "turtle.hpp"

struct LSystemViewer {
  LSystemViewer(LSystem &system) : system_{system} {}
  void drive(Turtle &turtle) {
    while (true) {
      auto ele = system_.next();
      CHECK(ele.depth_ <= 0);
      if (ele.depth_ == 0) {
        if (ele.data_ == "[") {
          turtle_stack_.push(turtle);
          continue;
        }
        if (ele.data_ == "]") {
          turtle = turtle_stack_.top();
          turtle_stack_.pop();
          continue;
        }
        auto iter = drivers_.find(ele.data_);
        CHECK_WITH_INFO(iter != drivers_.end(), ele.data_);
        iter->second->drive(turtle);
      } else {
        break;
      }
    }
  }
  void add_driver(const std::string &name, std::unique_ptr<Block> &&driver) {
    auto iter = drivers_.find(name);
    CHECK_WITH_INFO(iter == drivers_.end(), name);
    drivers_[name] = std::move(driver);
  }
  LSystem system_;
  std::stack<Turtle> turtle_stack_;
  std::unordered_map<std::string, std::unique_ptr<Block>> drivers_;
};