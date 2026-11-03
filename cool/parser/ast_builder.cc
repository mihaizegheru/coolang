#include "cool/parser/ast_builder.h"

#include <string>

#include "CoolParser.h"

#include "cool/ast/ast.h"

namespace cool {

void AstBuilder::Append(CoolParser::ProgramContext* tree,
                        const std::string& file_name, ast::Program& program) {
  // TODO(coolang): Visit class contexts, retain their
  // tokens/context/file_name, and append the resulting class nodes to
  // program.classes.
  (void)tree;
  (void)file_name;
  (void)program;
}

}  // namespace cool
