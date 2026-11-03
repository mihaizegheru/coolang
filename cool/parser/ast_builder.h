#ifndef COOLANG_COOL_PARSER_AST_BUILDER_H_
#define COOLANG_COOL_PARSER_AST_BUILDER_H_

#include <string>

#include "CoolParser.h"
#include "CoolParserBaseVisitor.h"

#include "cool/ast/ast.h"

namespace cool {

// Converts ANTLR contexts to AST nodes. This generated visitor adapter is
// private.
class AstBuilder : public CoolParserBaseVisitor {
 public:
  // Appends one input file's classes to the global program.
  void Append(CoolParser::ProgramContext* tree, const std::string& file_name,
              ast::Program& program);
  // TODO(coolang): Override generated visitXxx methods to construct the AST.
};

}  // namespace cool

#endif  // COOLANG_COOL_PARSER_AST_BUILDER_H_
