#pragma once
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "segment.hpp"
#include "vec.hpp"
float clamp(float x, float l, float r) {
  if (x < l) {
    return l;
  } else if (x <= r) {
    return x;
  } else {
    return r;
  }
}
struct PPMDumper {
  static std::vector<Segment>& outputSegments() {
    static std::vector<Segment> segments_;
    return segments_;
  }
  static void dump(const std::string name, uint32_t width, uint32_t height) {
#define IMAGE_REF(x, y) image_[(height - 1 - y) * width + x]
    std::vector<int> image_;
    image_.resize(width * height, 0);
    auto& segment_list = outputSegments();
    for (auto& segement : segment_list) {
      auto pixel_positions = to_pixel(segement, width - 1, height - 1);
      for (auto& pos : pixel_positions) {
        // std::cout << pos.x_ << " " << pos.y_ << " "
        //           << (height - pos.y_) * width + pos.x_ << " " << width *
        //           height
        //           << "\n";
        IMAGE_REF(pos.x_, pos.y_) = 255;
      }
    }
#undef IMAGE_REF
    std::ofstream ofs(name + ".ppm");
    ofs << "P3\n" << width << ' ' << height << "\n255\n";
    for (auto& color : image_) {
      ofs << color << ' ' << color << ' ' << color << '\n';
    }
  }
  static std::vector<Veci> to_pixel(const Segment& segment, uint32_t width,
                                    uint32_t height) {
    auto step = cal_max_step(segment, width, height);
    std::vector<Veci> pixel_postions;
    pixel_postions.reserve(step);
    auto segment_vec = segment.end_ - segment.start_;
    for (int i = 0; i <= step; i++) {
      auto point = segment.start_ + segment_vec * (float(i) / step);
      pixel_postions.push_back(to_pixel_position(point, width, height));
    }
    return pixel_postions;
  }
  static Veci to_pixel_position(const Vecf& coord, uint32_t width,
                                uint32_t height) {
    return make_vec<int>(int(clamp(coord.x_, 0.0f, 1.0f) * width),
                         int(clamp(coord.y_, 0.0f, 1.0f) * height));
  }
  static int32_t cal_max_step(const Segment& segment, uint32_t width,
                              uint32_t height) {
    Vecf v = segment.end_ - segment.start_;
    float abs_dx = std::fabs(v.x_);
    float abs_dy = std::fabs(v.y_);
    if (abs_dx > abs_dy) {
      return int32_t(width * abs_dx);
    } else {
      return int32_t(height * abs_dy);
    }
  }
};

void draw_line(const Vecf& start, const Vecf& end) {
  std::cout << "draw_line "
            << "(" << start.x_ << "," << start.y_ << ") --> (" << end.x_ << ","
            << end.y_ << ")\n";
  PPMDumper::outputSegments().push_back(make_segment(start, end));
}