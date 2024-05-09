#pragma once
#include "frame.hpp"
#include "painter.hpp"
#include "vec.hpp"
Painter transform_painter(const Painter& painter, const Vecf& origin,
                          const Vecf& corner0, const Vecf& corner1) {
  return [=](const Frame& frame) {
    auto frame_map_fun = frame_coord_map(frame);
    auto new_origin = frame_map_fun(origin);
    auto new_frame = make_frame(new_origin, frame_map_fun(corner0) - new_origin,
                                frame_map_fun(corner1) - new_origin);
    painter(new_frame);
  };
}
Painter flip_vert(const Painter& painter) {
  return transform_painter(painter, make_vec(0.0f, 1.0f), make_vec(1.0f, 1.0f),
                           make_vec(0.0f, 0.0f));
}
Painter flip_hori(const Painter& painter) {
  return transform_painter(painter, make_vec(1.0f, 0.0f), make_vec(0.0f, 0.0f),
                           make_vec(1.0f, 1.0f));
}
Painter shrink_to_upper_right(const Painter& painter) {
  return transform_painter(painter, make_vec(0.5f, 0.5f), make_vec(1.0f, 0.5f),
                           make_vec(0.5f, 1.0f));
}
Painter rotate90_counterclockwise(const Painter& painter) {
  return transform_painter(painter, make_vec(1.0f, 0.0f), make_vec(1.0f, 1.0f),
                           make_vec(0.0f, 0.0f));
}
Painter squash_inwards(const Painter& painter) {
  return transform_painter(painter, make_vec(0.0f, 0.0f),
                           make_vec(0.65f, 0.35f), make_vec(0.35f, 0.65f));
}

Painter beside(const Painter& painter0, const Painter& painter1) {
  auto split_point = make_vec(0.5f, 0.0f);
  auto painter_left = transform_painter(painter0, make_vec(0.0f, 0.0f),
                                        split_point, make_vec(0.0f, 1.0f));
  auto painter_right = transform_painter(
      painter1, split_point, make_vec(1.0f, 0.0f), make_vec(0.5f, 1.0f));
  return [=](const Frame& frame) {
    painter_left(frame);
    painter_right(frame);
  };
}
Painter below(const Painter& painter0, const Painter& painter1) {
  auto split_point = make_vec(0.0f, 0.5f);
  auto painter_top = transform_painter(
      painter0, split_point, make_vec(1.0f, 0.5f), make_vec(0.0f, 1.0f));
  auto painter_bottom = transform_painter(painter1, make_vec(0.0f, 0.0f),
                                          make_vec(1.0f, 0.0f), split_point);
  return [=](const Frame& frame) {
    painter_top(frame);
    painter_bottom(frame);
  };
}

Painter wave2(const Painter& painter) {
  return beside(painter, flip_vert(painter));
}

Painter wave4(const Painter& painter) {
  return below(wave2(painter), wave2(painter));
}

Painter right_split(const Painter& painter, uint32_t n) {
  if (n == 0) {
    return painter;
  }
  auto smaller = right_split(painter, n - 1);
  return beside(painter, below(smaller, smaller));
}

Painter left_split(const Painter& painter, uint32_t n) {
  // if (n == 0) {
  //   return painter;
  // }
  // auto smaller = left_split(painter, n - 1);
  // return beside(below(smaller, smaller), painter);
  return flip_hori(right_split(painter, n));
}
Painter up_split(const Painter& painter, uint32_t n) {
  // if (n == 0) {
  //   return painter;
  // }
  // auto smaller = up_split(painter, n - 1);
  // return below(beside(smaller, smaller), painter);
  return rotate90_counterclockwise(right_split(painter, n));
}
Painter bottom_split(const Painter& painter, uint32_t n) {
  // if (n == 0) {
  //   return painter;
  // }
  // auto smaller = bottom_split(painter, n - 1);
  // return below(painter, beside(smaller, smaller));
  return flip_vert(up_split(painter, n));
}

Painter corner_split(const Painter& painter, uint32_t n) {
  if (n == 0) {
    return painter;
  }
  auto up = up_split(painter, n - 1);
  auto right = right_split(painter, n - 1);
  auto top_left = beside(up, up);
  auto bottom_right = below(right, right);
  auto corner = corner_split(painter, n - 1);
  return beside(below(top_left, painter), below(corner, bottom_right));
}

Painter square_limit(const Painter& painter, uint32_t n) {
  auto quarter = corner_split(painter, n);
  auto half = beside(flip_hori(quarter), quarter);
  return below(half, flip_vert(half));
}
