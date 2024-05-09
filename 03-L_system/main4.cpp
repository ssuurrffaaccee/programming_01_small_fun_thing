#include "l_system.hpp"
#include "l_system_viewer.hpp"
#include <iostream>

int main() {
  try {

    LSystem system;
    system.init({"F"}, 5);
    system.add_rule("F", {"F","[", "+", "F","]","F","[", "-", "F","]","[","F","]"});
    LSystemViewer system_viewer{system};
    uint32_t dump_size{1000};
    float one_step_length = 15.0f / dump_size;
    Turtle turtle = defaul_turtle(make_vec(0.5f,0.0f),one_step_length);
    float degree = 20;
    system_viewer.add_driver("F", forward_steps(1));
    system_viewer.add_driver("-", right_degrees(degree));
    system_viewer.add_driver("+", right_degrees(-degree));
    system_viewer.drive(turtle);
    draw_axis();
    PPMDumper::dump("l", dump_size, dump_size);
  } catch (MyExceptoin &e) {
    std::cout << e.what() << "\n";
  }
  return 0;
}