#pragma once
#include "vec.hpp"
struct Segment {
  Vecf start_{};
  Vecf end_{};
};

Segment make_segment(const Vecf& s, const Vecf& e) { return Segment{s, e}; }
