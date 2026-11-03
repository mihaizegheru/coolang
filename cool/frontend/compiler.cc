#include "cool/frontend/compiler.h"

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "cool/ast/ast.h"
#include "cool/ast/ast_printer.h"
#include "cool/parser/parsing.h"

namespace cool {

int Compile(const std::vector<std::string>& files, bool tokens_only) {
  if (files.empty()) {
    std::cerr << "No file(s) given\n";
    return 0;
  }
  std::vector<std::unique_ptr<Parsing>> inputs;
  bool errors = false;
  for (const auto& file : files) {
    auto parsed = std::make_unique<Parsing>(file);
    if (!parsed->LoadFile()) {
      std::cerr << "Cannot open input file: " << file << '\n';
      return 1;
    }
    if (tokens_only) {
      parsed->DumpTokens(std::cout);
    } else {
      parsed->Parse();
    }
    errors |= parsed->has_errors();
    inputs.push_back(std::move(parsed));
  }
  if (tokens_only) {
    return 0;
  }
  if (errors) {
    std::cerr << "Compilation halted\n";
    return 0;
  }
  // Parsing sessions outlive the AST's token/context references.
  ast::Program program;
  for (const auto& parsed : inputs) {
    parsed->AppendTo(program);
  }
  AstPrinter(std::cout).Print(program);
  return 0;
}

}  // namespace cool
