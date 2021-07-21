#include <iostream>

#include "src/nuri_evauator.hpp"

int main(int argc, char * argv[]) {

  constexpr auto a = ne::eval(U"안녕하세요");

  std::cout << a << std::endl;

  return 0;
}