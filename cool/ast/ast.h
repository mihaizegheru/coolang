#ifndef COOLANG_COOL_AST_AST_H_
#define COOLANG_COOL_AST_AST_H_

#include <memory>
#include <string>
#include <vector>

#include "ParserRuleContext.h"
#include "Token.h"

namespace cool::ast {

// TODO(coolang): Add typed nodes and a visitor interface as the grammar
// grows. Token/context pointers are non-owning. The frontend retains all
// parsing sessions until Compile() returns, keeping these pointers valid during
// printing.
struct Node {
  antlr4::Token* token = nullptr;
  antlr4::ParserRuleContext* context = nullptr;
  std::string file_name;
  virtual ~Node() = default;
};

struct Program final : Node {
  std::vector<std::unique_ptr<Node>> classes;
};

}  // namespace cool::ast

#endif  // COOLANG_COOL_AST_AST_H_
