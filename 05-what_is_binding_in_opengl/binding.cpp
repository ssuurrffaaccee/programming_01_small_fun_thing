#include <iostream>
#include <map>
#include <string>
#include <vector>
std::map<size_t, void*> G;
size_t get_index() {
  static size_t I = 0;
  return I++;
}
#define G_STR 0
#define G_INT 1
std::vector<size_t> P{0, 0};
size_t GenString() {
  auto i = get_index();
  G[i] = (void*)new std::string{};
  return i;
}
size_t GenInt() {
  auto i = get_index();
  G[i] = (void*)new int{0};
  return i;
}
std::string& GetString() {
  auto i = P[G_STR];
  return *(std::string*)G[i];
}
int& GetInt() {
  auto i = P[G_INT];
  return *(int*)G[i];
}
void Bind(size_t postion, size_t container) { P[postion] = container; }

void AddInt(size_t num) { GetInt() += num; }
void AddChar(char c) { GetString().push_back(c); }
int main() {
  auto int0 = GenInt();
  auto int1 = GenInt();
  {
    Bind(G_INT, int0);
    AddInt(10);
    std::cout << GetInt() << "\n";
  }
  {
    Bind(G_INT, int1);
    AddInt(2);
    std::cout << GetInt() << "\n";
  }
  {
    auto s = GenString();
    Bind(G_STR, s);
    AddChar('a');
    std::cout << GetString() << "\n";
  }
  return 0;
}
