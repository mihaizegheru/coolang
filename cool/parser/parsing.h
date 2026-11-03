#ifndef COOLANG_COOL_PARSER_PARSING_H_
#define COOLANG_COOL_PARSER_PARSING_H_

#include <iosfwd>
#include <memory>
#include <string>

#include "cool/ast/ast.h"

namespace cool {

// Owns one source file's tokens and parse tree. Keep this session alive while
// using AST nodes that reference its tokens or contexts. ANTLR details stay
// private.
class Parsing {
 public:
  explicit Parsing(const std::string& file_name);
  ~Parsing();
  Parsing(const Parsing&) = delete;
  Parsing& operator=(const Parsing&) = delete;

  // Returns false if the input file cannot be opened. Call before
  // parsing/dumping.
  bool LoadFile();
  void Parse();
  void DumpTokens(std::ostream& output);
  bool has_errors() const;
  // Call after Parse(), once lexical/syntax errors have been checked.
  void AppendTo(ast::Program& program);

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace cool

#endif  // COOLANG_COOL_PARSER_PARSING_H_
