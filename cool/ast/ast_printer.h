#ifndef COOLANG_COOL_AST_AST_PRINTER_H_
#define COOLANG_COOL_AST_AST_PRINTER_H_

#include <cstddef>
#include <ostream>
#include <string>

#include "cool/ast/ast.h"

namespace cool {

// Prints the hierarchical AST representation. The output must outlive this
// object.
class AstPrinter {
 public:
  explicit AstPrinter(std::ostream& output) : output_(output) {}
  void Print(const ast::Program& program);

 private:
  void PrintLine(const std::string& text, std::size_t depth);
  std::ostream& output_;
};

}  // namespace cool

#endif  // COOLANG_COOL_AST_AST_PRINTER_H_
