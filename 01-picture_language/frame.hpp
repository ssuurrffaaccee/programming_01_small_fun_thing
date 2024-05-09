#pragma once
#include "vec.hpp"
struct Frame {
  Vecf origin_{};
  Vecf edge_x_{};
  Vecf edge_y_{};
};

Frame std_frame() {
  return Frame{make_vec(0.0f, 0.0f), make_vec(1.0f, 0.0f),
               make_vec(0.0f, 1.0f)};
}

Frame make_frame(const Vecf& origin, const Vecf& edge_x, const Vecf& edge_y) {
  return Frame{origin, edge_x, edge_y};
}

auto frame_coord_map(const Frame& frame) {
  return [frame](const Vecf& v) {
    return frame.origin_ + frame.edge_x_ * v.x_ + frame.edge_y_ * v.y_;
  };
}
