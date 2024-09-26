#include <ucontext.h>

#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
struct Random {
  static void init() { srand(static_cast<unsigned int>(time(0))); }
  static int rand(int x) { return ::rand() % x; }
};

#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0)

////CONFIG/////////////
#define WORK_NUM 4
#define STACK_SIZE 1024*10
//////////////////////

struct Scheduler {
  template <int Index>
  struct WorkerContext {
    static ucontext_t u_context_;
    static std::vector<char> stack_;
    static bool is_working_;
    static void init(const int stack_size) {
      std::cout << "init worker\n";
      if (getcontext(&u_context_) == -1) {
        handle_error("init worker failed");
      }
      stack_.resize(stack_size);
      u_context_.uc_stack.ss_sp = stack_.data();
      u_context_.uc_stack.ss_size = stack_size;
      u_context_.uc_link = nullptr;
      makecontext(&u_context_, &worker_work, 0);
    }
    static void worker_work() {
      std::cout << "begin worker_work\n";
      while (!Scheduler::is_stop) {
        if (!Scheduler::global_work_queue_[Index].empty()) {
          auto& work = Scheduler::global_work_queue_[Index].front();
          is_working_ = true;
          bool is_done{false};
          work(is_done, &yield_to_scheduler);
          is_working_ = false;
          if (is_done) {
            Scheduler::global_work_queue_[Index].pop();
          }
        }
        yield_to_scheduler();
      }
    }
    static void yield_to_scheduler() {
      swapcontext(&u_context_, &SchedulerContext::u_context_);
    }
    static void switch_to() {
      swapcontext(&SchedulerContext::u_context_, &u_context_);
    }
    static bool is_working() { return is_working_; }
  };
  struct SchedulerContext {
    static ucontext_t u_context_;
    static std::vector<char> stack_;
    static void init(const int stack_size) {
      std::cout << "init scheduler\n";
      if (getcontext(&u_context_) == -1) {
        handle_error("init scheduler failed");
      }
      stack_.resize(stack_size);
      u_context_.uc_stack.ss_sp = stack_.data();
      u_context_.uc_stack.ss_size = stack_size;
      u_context_.uc_link = &main_context_;
      makecontext(&u_context_, &scheduler_work, 0);
    }
    static void scheduler_work() {
      std::cout << "begin scheduler_work\n";
      while (is_need_working()) {
        int random_int = Random::rand(WORK_NUM);
        Scheduler::switch_to_funcs[random_int]();
      }
    }
    static bool is_need_working() {
      if (!Scheduler::is_stop) {
        bool is_need_working{false};
        for (auto& is_working_func : Scheduler::is_working_funcs) {
          is_need_working = is_need_working || is_working_func();
        }
        for (int i{0}; i < WORK_NUM; i++) {
          is_need_working =
              is_need_working || !Scheduler::global_work_queue_[i].empty();
        }
        return is_need_working;
      }
      return false;
    }
  };

  static std::queue<std::function<void(bool&, void (*)())>>
      global_work_queue_[WORK_NUM];
  static bool is_stop;
  static ucontext_t main_context_;
  static int worker_num;
  static std::vector<std::function<void()>> switch_to_funcs;
  static std::vector<std::function<bool()>> is_working_funcs;
  static void init() { _init<WORK_NUM>(); }
  template <int WorkerNum>
  static void _init() {
    SchedulerContext::init(STACK_SIZE);
    init_workers(STACK_SIZE, std::make_integer_sequence<int, WorkerNum>());
  }
  template <typename T, T... I>
  static void init_workers(int stack_size, std::integer_sequence<T, I...>) {
    (WorkerContext<I>::init(stack_size), ...);
    (Scheduler::switch_to_funcs.push_back(&WorkerContext<I>::switch_to), ...);
    (Scheduler::is_working_funcs.push_back(&WorkerContext<I>::is_working), ...);
  }
  static void run(const std::function<void(bool&, void (*)())>& func) {
    int random_int = Random::rand(WORK_NUM);
    global_work_queue_[random_int].push(func);
  }
  static void wait() {
    swapcontext(&main_context_, &SchedulerContext::u_context_);
  }
};
std::queue<std::function<void(bool&, void (*)())>>
    Scheduler::global_work_queue_[WORK_NUM];
bool Scheduler::is_stop{false};
ucontext_t Scheduler::main_context_;
std::vector<std::function<void()>> Scheduler::switch_to_funcs;
std::vector<std::function<bool()>> Scheduler::is_working_funcs;
ucontext_t Scheduler::SchedulerContext::u_context_;
std::vector<char> Scheduler::SchedulerContext::stack_;
template <int Index>
ucontext_t Scheduler::WorkerContext<Index>::u_context_;
template <int Index>
std::vector<char> Scheduler::WorkerContext<Index>::stack_;
template <int Index>
bool Scheduler::WorkerContext<Index>::is_working_{false};

int main() {
  Scheduler::init();
  std::cout << "Scheduler::init done\n";
  Scheduler::run([](bool& is_done, void (*yield)()) {
    std::cout << "main work:" << "\n";
    for (int i{0}; i < 5; i++) {
      Scheduler::run([i](bool& is_done, void (*yield)()) {
        std::cout << "do sub work:" << i << "\n";
        is_done = true;
      });
      yield();
      std::cout << "gen sub work:" << i << "\n";
    }
    is_done = true;
  });
  std::cout << "Scheduler::wait begin\n";
  Scheduler::wait();
  return 0;
}