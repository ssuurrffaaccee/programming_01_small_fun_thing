#pragma once
#include <functional>
#include <vector>

#include "draw_line.hpp"
#include "frame.hpp"
#include "segment.hpp"
using Painter = std::function<void(const Frame&)>;
Painter segment_painter(const std::vector<Segment>& segment_list) {
  return [segment_list](const Frame& frame) {
    auto frame_map_func = frame_coord_map(frame);
    for (auto& segment : segment_list) {
      auto start_in_frame = frame_map_func(segment.start_);
      auto end_in_frame = frame_map_func(segment.end_);
      draw_line(start_in_frame, end_in_frame);
    }
  };
}
