#pragma once
#include <bitset>
#include <string>
#include <vector>

#include "check.hpp"
#include "sptr.hpp"
class Wire {
 public:
  using T = uint8_t;
  Wire() : signal_value_{}, is_first_time_{true} {}
  Wire(T signal_value) : signal_value_{signal_value}, is_first_time_{true} {}
  ~Wire() {}
  T get_signal() { return signal_value_; }
  void set_signal(T signal_value) {
    signal_value = signal_value&mask_;
    if (signal_value_ != signal_value || is_first_time_) {
      is_first_time_ = false;
      signal_value_ = signal_value;
      for (auto& action : actions_) {
        action();
      }
    }
  }
  void add_action(const std::function<void()>& action) {
    actions_.push_back(action);
  }

  std::string to_string() {
    constexpr size_t bit_nums = 1;
    std::bitset<bit_nums> b{signal_value_};
    return b.to_string();
  }

 private:
  std::vector<std::function<void()>> actions_;
  T signal_value_;
  bool is_first_time_;
  static T mask_;
};
Wire::T Wire::mask_{1};


const Wire::T TRUE = 0xFF;
const Wire::T FALSE = 0x00;


SPtr<Wire> make_wire() {
  return std::make_shared<Wire>();
}

SPtr<Wire> make_wire(uint8_t signal_value) {
  return std::make_shared<Wire>(signal_value);
}
