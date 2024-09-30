#include <lib/number.h>
#include <iostream>

int main() {
  // int2023_t k1 = from_string("-123456789123456789");
  // int2023_t k2 = from_string("-27358325");

  // int2023_t v = (k1 / k2);

  // std::cout << k1 << " " << k2 << " " << v << std::endl;

  int2023_t a = from_int(-2147483648);
  int2023_t b = from_string("2147483648");
  std::cout << (a == b) << '\n';

  return 0;
}
