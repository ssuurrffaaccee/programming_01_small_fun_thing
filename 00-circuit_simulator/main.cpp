#include <iostream>
#include <sstream>

#include "gate.hpp"
std::string to_string(std::vector<SPtr<Wire>>& bus) {
  std::stringstream ss;
  for (auto& wire : bus) {
    ss << wire->to_string();
  }
  return ss.str();
}
int main() {
  try {
    {
      std::cout << "and gate\n";
      auto a = make_wire();
      auto b = make_wire();
      auto c = and_(a, b);
      a->set_signal(TRUE);
      b->set_signal(TRUE);
      Agenda::get().propagate();
      std::cout << a->to_string() << "\n";
      std::cout << b->to_string() << "\n";
      std::cout << c->to_string() << "\n";
    }
    std::cout << "=============================\n";
    {
      std::cout << "or gate\n";
      auto a = make_wire();
      auto b = make_wire();
      auto c = or_(a, b);
      a->set_signal(TRUE);
      b->set_signal(FALSE);
      Agenda::get().propagate();
      std::cout << a->to_string() << "\n";
      std::cout << b->to_string() << "\n";
      std::cout << c->to_string() << "\n";
    }
    std::cout << "=============================\n";
    {
      std::cout << "not gate\n";
      auto a = make_wire();
      auto b = not_(a);
      a->set_signal(TRUE);
      Agenda::get().propagate();
      std::cout << a->to_string() << "\n";
      std::cout << b->to_string() << "\n";
    }
    std::cout << "=============================\n";
    {
      std::cout << "half-adder gate\n";
      auto a = make_wire();
      auto b = make_wire();
      auto [sum, carry] = half_adder(a, b);
      a->set_signal(TRUE);
      b->set_signal(TRUE);
      Agenda::get().propagate();
      std::cout << a->to_string() << "\n";
      std::cout << b->to_string() << "\n";
      std::cout << sum->to_string() << "\n";
      std::cout << carry->to_string() << "\n";
    }
  } catch (MyExceptoin& e) {
    std::cout << e.what() << '\n';
  }
  return 0;
}