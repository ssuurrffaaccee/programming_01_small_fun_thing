#include "ops.hpp"
int main() {
  std::vector<Segment> segment_list;
  segment_list.push_back(
      make_segment(make_vec(0.0f, 0.0f), make_vec(1.0f, 1.0f)));
  segment_list.push_back(
      make_segment(make_vec(1.0f, 0.0f), make_vec(0.0f, 1.0f)));
  auto painter = segment_painter(segment_list);
  painter = square_limit(painter, 8);
  auto frame = std_frame();
  painter(frame);
  PPMDumper::dump("square_limit", 1000, 1000);
  return 0;
}