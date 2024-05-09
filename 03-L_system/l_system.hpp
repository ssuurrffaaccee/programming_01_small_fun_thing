#pragma once
#include "check.hpp"
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
struct Element {
  int depth_{-1};
  std::string data_{};
};
struct LSystem {
  void init(const std::vector<std::string> &init_str, int depth) {
    std::vector<Element> init_elements;
    for (int i = init_str.size() - 1; i >= 0; i--) {
      stack_.push(Element{depth, init_str[i]});
    }
  }
  void add_rule(const std::string &left,
                const std::vector<std::string> &right) {
    std::vector<Element> rule_right_hand;
    rule_right_hand.resize(right.size());
    for (int i = 0; i < right.size(); i++) {
      rule_right_hand[i].data_ = right[i];
    }
    auto iter = rules_.find(left);
    CHECK_WITH_INFO(iter == rules_.end(), left);
    rules_[left] = rule_right_hand;
  }
  Element next() {
    if (!stack_.empty()) {
      Element element = stack_.top();
      stack_.pop();
      while (element.depth_ > 0) {
        if (try_apply_rule(element)) {
          element = stack_.top();
          stack_.pop();
        } else {
          element.depth_ = 0;
          break;
        }
      }
      return element;
    }

    return Element{};
  }
  bool try_apply_rule(const Element &element) {
    auto iter = rules_.find(element.data_);
    // CHECK_WITH_INFO(iter != rules_.end(), element.data_);
    if (iter == rules_.end()) {
      return false;
    }
    std::vector<Element> rule_right_hand = iter->second;
    auto next_depth = element.depth_ - 1;
    for (auto &e : rule_right_hand) {
      e.depth_ = next_depth;
    }
    for (int i = rule_right_hand.size() - 1; i >= 0; i--) {
      stack_.push(rule_right_hand[i]);
    }
    return true;
  }
  std::stack<Element> stack_;
  std::unordered_map<std::string, std::vector<Element>> rules_;
};