#pragma once
#include <string>
#include <vector>
#include <cctype>
#include <cstring>
#include "Token.h"

class Lexer {
public:
  explicit Lexer(const std::string& input);

  Token getNextToken();
  bool hasMoreTokens() const;

private:
  std::string input;
  size_t pos;
  size_t max_pos;
  int line;
  int col;

  void skipWhitespace();
  bool skipComments();
};