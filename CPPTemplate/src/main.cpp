#include "helloWorld.h"
#include <iostream>

int main() {
  HelloWorld* hw = new HelloWorld;
  hw->Run();
  std::cout << hw->Square(4) << std::endl;
  delete hw;
  return 0;
}
