#include "cool/parser/parsing.h"

#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "ANTLRInputStream.h"
#include "BaseErrorListener.h"
#include "CommonTokenStream.h"
#include "CoolLexer.h"
#include "CoolParser.h"
#include "Token.h"

#include "cool/ast/ast.h"
#include "cool/parser/ast_builder.h"

namespace cool {
namespace {

class ErrorListener final : public antlr4::BaseErrorListener {
 public:
  explicit ErrorListener(std::string file_name)
      : file_name_(std::move(file_name)) {}

  // ANTLR requires this exact override name and signature.
  void syntaxError(antlr4::Recognizer*, antlr4::Token* token, std::size_t line,
                   std::size_t column, const std::string& message,
                   std::exception_ptr) override {
    const auto separator = file_name_.find_last_of('/');
    const std::string name = separator == std::string::npos
                                 ? file_name_
                                 : file_name_.substr(separator + 1);
    std::cerr << '"' << name << "\", line " << line << ':' << column + 1
              << ", ";
    if (token && token->getType() == CoolLexer::ERROR) {
      std::cerr << "Lexical error: " << token->getText();
    } else {
      std::cerr << "Syntax error: " << message;
    }
    std::cerr << '\n';
    errors_ = true;
  }

  bool has_errors() const { return errors_; }

 private:
  std::string file_name_;
  bool errors_ = false;
};

}  // namespace

struct Parsing::Impl {
  std::string file_name;
  ErrorListener listener;
  antlr4::ANTLRInputStream input;
  CoolLexer lexer{&input};
  antlr4::CommonTokenStream tokens{&lexer};
  CoolParser parser{&tokens};
  CoolParser::ProgramContext* tree = nullptr;

  explicit Impl(const std::string& path) : file_name(path), listener(path) {
    // TODO(coolang): Lexical failures must become ERROR tokens through
    // grammar rules rather than ANTLR recognition errors. These diagnostics are
    // scaffolding.
    lexer.removeErrorListeners();
    lexer.addErrorListener(&listener);
    parser.removeErrorListeners();
    parser.addErrorListener(&listener);
  }
};

Parsing::Parsing(const std::string& file_name)
    : impl_(std::make_unique<Impl>(file_name)) {}

Parsing::~Parsing() = default;

bool Parsing::LoadFile() {
  std::ifstream stream(impl_->file_name, std::ios::binary);
  if (!stream) {
    return false;
  }
  impl_->input.load(stream);
  impl_->input.name = impl_->file_name;
  return true;
}

void Parsing::Parse() { impl_->tree = impl_->parser.program(); }

void Parsing::DumpTokens(std::ostream& output) {
  impl_->tokens.fill();
  for (const auto* token : impl_->tokens.getTokens()) {
    const auto type = token->getType();
    const auto name = type == antlr4::Token::EOF
                          ? "EOF"
                          : impl_->lexer.getVocabulary().getSymbolicName(type);
    output << token->getText() << " : " << name << '\n';
  }
}

bool Parsing::has_errors() const { return impl_->listener.has_errors(); }

void Parsing::AppendTo(ast::Program& program) {
  AstBuilder builder;
  builder.Append(impl_->tree, impl_->file_name, program);
}

}  // namespace cool
