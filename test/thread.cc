#include <iostream>

#include "jthread.h"

int main(int argc, char **argv) {
  yf::jthread thread{[]() { std::cout << "hello jthread\n"; }};
  return 0;
}