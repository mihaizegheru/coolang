#include "cool/ast/ast_printer.h"

#include <cstddef>
#include <string>

#include "cool/ast/ast.h"

namespace cool {

void AstPrinter::PrintLine(const std::string& text, std::size_t depth) {
  output_ << std::string(depth * 2, ' ') << text << '\n';
}

void AstPrinter::Print(const ast::Program& program) {
  PrintLine("program", 0);
  // TODO(coolang): Visit program.classes and print the required AST
  // representation.
  (void)program;
}

}  // namespace cool
