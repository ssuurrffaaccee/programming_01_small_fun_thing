#include "l_system.hpp"
#include "l_system_viewer.hpp"
#include <iostream>

int main() {
  try {

    LSystem system;
    system.init({"Fr"}, 6);
    system.add_rule("Fl", {"Fr", "+", "Fl","+","Fr"});
    system.add_rule("Fr", {"Fl", "-", "Fr","-","Fl"});
    LSystemViewer system_viewer{system};
    uint32_t dump_size{1000};
    float one_step_length = 10.0f / dump_size;
    Turtle turtle = defaul_turtle(make_vec(0.7f,0.18f),one_step_length);
    float degree = 60;
    system_viewer.add_driver("Fr", forward_steps(1));
    system_viewer.add_driver("Fl", forward_steps(1));
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