#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "cool/frontend/compiler.h"

int main(int argc, char** argv) {
  bool tokens_only = false;
  std::vector<std::string> files;
  bool parse_options = true;
  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (parse_options && arg == "--") {
      parse_options = false;
      continue;
    }
    if (parse_options && arg == "--tokens") {
      tokens_only = true;
      continue;
    }
    if (parse_options && (arg == "--help" || arg == "-h")) {
      std::cout << "Usage: coolc [--tokens] [--] FILE.cl [FILE.cl ...]\n";
      return 0;
    }
    files.push_back(arg);
  }
  // ANTLR can throw. Contain dependency exceptions at the executable boundary.
  try {
    return cool::Compile(files, tokens_only);
  } catch (const std::exception& error) {
    std::cerr << error.what() << '\n';
    return 1;
  }
}
