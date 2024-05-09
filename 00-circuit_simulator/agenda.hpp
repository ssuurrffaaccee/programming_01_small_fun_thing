#pragma once
#include <functional>
#include <queue>
#include <unordered_map>

#include "check.hpp"
#include "sptr.hpp"

class Agenda {
 public:
  static Agenda &get() {
    static Agenda agenda_;
    return agenda_;
  }
  void after_delay(uint64_t delay, const std::function<void()> &action) {
    // std::cout << "inert time: " << current_time_ + delay << "\n";
    add_to_agenda(current_time_ + delay, action);
  }
  void propagate() {
    while (!segments_.empty()) {
      // std::cout << "time: " << current_time_
      //           << " segments_num: " << segments_.size() << "\n";
      auto segment_queue_ptr = segments_.top();
      segments_.pop();
      current_time_ = segment_queue_ptr->time_;
      auto &actions = segment_queue_ptr->actions_;
      while (!actions.empty()) {
        auto &top_action = actions.front();
        top_action();
        actions.pop();
      }
      segment_index_.erase(segment_queue_ptr->time_);
    }
  }

 private:
  void add_to_agenda(uint64_t time, const std::function<void()> &action) {
    auto iter = segment_index_.find(time);
    if (iter == segment_index_.end()) {
      auto segment_queue = SegmentQueue::make(time);
      segments_.push(segment_queue.get());
      auto emplace_result =
          segment_index_.insert(std::make_pair(time, std::move(segment_queue)));
      CHECK(emplace_result.second)
      iter = emplace_result.first;
    }
    iter->second->actions_.push(action);
  }
  struct SegmentQueue {
    static std::unique_ptr<SegmentQueue> make(uint64_t time) {
      auto segment_queue = std::make_unique<SegmentQueue>();
      segment_queue->time_ = time;
      return segment_queue;
    }
    int time_;
    std::queue<std::function<void()>> actions_;

    static bool comp_func(const SegmentQueue *q1, const SegmentQueue *q2) {
      return q1->time_ < q2->time_;
    };
  };
  using PrioritySegment =
      std::priority_queue<SegmentQueue *, std::vector<SegmentQueue *>,
                          decltype(&SegmentQueue::comp_func)>;
  Agenda()
      : current_time_{0}, segments_{PrioritySegment{SegmentQueue::comp_func}} {}
  uint64_t current_time_;
  std::unordered_map<uint64_t, std::unique_ptr<SegmentQueue>> segment_index_;
  PrioritySegment segments_;
};