#pragma once
#include "agenda.hpp"
#include "wire.hpp"
static uint16_t NORMAL_GAGE_DELAY = 1;

SPtr<Wire> and_(const SPtr<Wire> &a, const SPtr<Wire> &b) {
  auto and_res = make_wire();
  auto and_action = [=]() {
    auto logcial_and_res = a->get_signal() & b->get_signal();
    Agenda::get().after_delay(NORMAL_GAGE_DELAY,
                              [=]() { and_res->set_signal(logcial_and_res); });
  };
  a->add_action(and_action);
  b->add_action(and_action);
  return and_res;
}

void and_(const SPtr<Wire> &a, const SPtr<Wire> &b, const SPtr<Wire> &and_res) {
  auto and_action = [=]() {
    auto logcial_and_res = a->get_signal() & b->get_signal();
    Agenda::get().after_delay(NORMAL_GAGE_DELAY,
                              [=]() { and_res->set_signal(logcial_and_res); });
  };
  a->add_action(and_action);
  b->add_action(and_action);
}

SPtr<Wire> not_(const SPtr<Wire> &a) {
  auto not_res = make_wire();
  auto not_action = [=]() {
    auto logcial_not_res = ~a->get_signal();
    Agenda::get().after_delay(NORMAL_GAGE_DELAY,
                              [=]() { not_res->set_signal(logcial_not_res); });
  };
  a->add_action(not_action);
  return not_res;
}

void not_(const SPtr<Wire> &a, const SPtr<Wire> &not_res) {
  auto not_action = [=]() {
    auto logcial_not_res = ~a->get_signal();
    Agenda::get().after_delay(NORMAL_GAGE_DELAY,
                              [=]() { not_res->set_signal(logcial_not_res); });
  };
  a->add_action(not_action);
}

SPtr<Wire> or_(const SPtr<Wire> &a, const SPtr<Wire> &b) {
  auto or_res = make_wire();
  auto or_action = [=]() {
    auto logcial_or_res = a->get_signal() | b->get_signal();
    Agenda::get().after_delay(NORMAL_GAGE_DELAY,
                              [=]() { or_res->set_signal(logcial_or_res); });
  };
  a->add_action(or_action);
  b->add_action(or_action);
  return or_res;
}

void or_(const SPtr<Wire> &a, const SPtr<Wire> &b, const SPtr<Wire> &or_res) {
  auto or_action = [=]() {
    auto logcial_or_res = a->get_signal() | b->get_signal();
    Agenda::get().after_delay(NORMAL_GAGE_DELAY,
                              [=]() { or_res->set_signal(logcial_or_res); });
  };
  a->add_action(or_action);
  b->add_action(or_action);
}

std::pair<SPtr<Wire>, SPtr<Wire>> half_adder(const SPtr<Wire> &a,
                                             const SPtr<Wire> &b) {
  auto carry_res = and_(a, b);
  auto sum_res = and_(or_(a, b), not_(carry_res));
  return std::make_pair(sum_res, carry_res);
}

// void nor_latch(const SPtr<Wire> &R, const SPtr<Wire> &S, const SPtr<Wire> &Q)
// {
//   auto nor_latch_action = [=] {
//     auto R_signal_value = R->get_signal();
//     auto S_signal_value = S->get_signal();
//     auto Q_signal_value = Q->get_signal();
//     auto not_Q_signal_value = ~(S_signal_value | Q_signal_value);
//     Agenda::get().after_delay(NORMAL_GAGE_DELAY, [=]() {
//       Q->set_signal(~(R_signal_value | not_Q_signal_value));
//     });
//   };
//   R->add_action(nor_latch_action);
//   S->add_action(nor_latch_action);
// }
// void nand_latch(const SPtr<Wire> &R, const SPtr<Wire> &S, const SPtr<Wire>
// &Q) {
//   auto nand_latch_action = [=] {
//     auto R_signal_value = R->get_signal();
//     auto S_signal_value = S->get_signal();
//     auto Q_signal_value = Q->get_signal();
//     auto not_Q_signal_value = ~(S_signal_value & Q_signal_value);
//     Agenda::get().after_delay(NORMAL_GAGE_DELAY, [=]() {
//       Q->set_signal(~(R_signal_value & not_Q_signal_value));
//     });
//   };
//   R->add_action(nand_latch_action);
//   S->add_action(nand_latch_action);
// }

// void toggle(const SPtr<Wire> &c, const SPtr<Wire> &in, const SPtr<Wire> &out)
// {
//   auto toggle_action = [=] {
//     std::cout<<c->to_string()<<" "<<in->to_string()<<"
//     "<<out->to_string()<<"\n"; auto c_signal_value = c->get_signal(); auto
//     in_signal_value = in->get_signal(); auto out_signal_value =
//     out->get_signal(); if (bool(c_signal_value) && bool(in_signal_value)) {
//       out_signal_value = ~out_signal_value;
//     }
//     Agenda::get().after_delay(NORMAL_GAGE_DELAY,
//                               [=]() { out->set_signal(out_signal_value); });
//   };
//   c->add_action(toggle_action);
//   in->add_action(toggle_action);
// }

// void program_counter(const SPtr<Wire> &C_p, const SPtr<Wire> &CLK,
//                      const std::vector<SPtr<Wire>> &out) {
//   CHECK(!out.empty())
//   int toggle_num = out.size();
//   toggle(C_p, CLK, out[toggle_num - 1]);
//   for (int i = 1; i < toggle_num; i++) {
//     toggle(C_p, out[toggle_num - i], out[toggle_num - i - 1]);
//   }
// };