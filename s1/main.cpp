// stringstream::str
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream, std::stringbuf

int main () {
  std::stringstream ss;
  ss<< "aa";
  ss<<"bb";
  std::string s = ss.str();

  std::cout << s << '\n';
  return 0;
}
