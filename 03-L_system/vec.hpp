#pragma once
template <class T>
T get_default();

template <>
int get_default() {
  return 0;
}
template <>
float get_default() {
  return 0.0f;
}

template <class T>
struct Vec {
  T x_{get_default<T>()};
  T y_{get_default<T>()};
};

template <class T>
Vec<T> make_vec(T x, T y) {
  return Vec<T>{x, y};
}

template <class T>
Vec<T> operator+(const Vec<T>& l, const Vec<T>& r) {
  Vec<T> res;
  res.x_ = l.x_ + r.x_;
  res.y_ = l.y_ + r.y_;
  return res;
}
template <class T>
Vec<T> operator-(const Vec<T>& l, const Vec<T>& r) {
  Vec<T> res;
  res.x_ = l.x_ - r.x_;
  res.y_ = l.y_ - r.y_;
  return res;
}

template <class T>
Vec<T> operator*(const Vec<T>& l, float c) {
  Vec<T> res;
  res.x_ = l.x_ * c;
  res.y_ = l.y_ * c;
  return res;
}
template <class T>
Vec<T> operator*(float c, const Vec<T>& l) {
  Vec<T> res;
  res.x_ = l.x_ * c;
  res.y_ = l.y_ * c;
  return res;
}

using Vecf = Vec<float>;
using Veci = Vec<int>;