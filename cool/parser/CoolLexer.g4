lexer grammar CoolLexer;

tokens { ERROR }

@members {
  void RaiseError(const std::string& message) {
    setText(message);
    setType(ERROR);
  }
}

// TODO: Implement Cool tokens, keywords, literals, nested comments and errors.
// Actions here are C++, e.g. RaiseError("EOF in comment").
WS : [ \n\f\r\t]+ -> skip;
