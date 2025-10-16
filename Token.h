#pragma once
#include <string>
#include <unordered_set>
#include <algorithm>
#include <iomanip>   
#include <sstream>   

enum class TokenType {
  Identifier, Keyword, Literal, Operator, Symbol, EoF, EoL, Error,
};

static std::string tokenTypeToString(TokenType type) {
  switch (type) {
    case TokenType::Keyword:   return "Keyword";
    case TokenType::Identifier:return "Identifier";
    case TokenType::Literal:   return "Literal";
    case TokenType::Operator:  return "Operator";
    case TokenType::Symbol:    return "Symbol";
    case TokenType::EoL:       return "EoL";
    case TokenType::EoF:       return "EoF";
    case TokenType::Error:     return "Error";
    default:                   return "Unknown";
  }
}


static const std::unordered_set<std::string> IEEE_1076_VHDL_KEYWORDS = {
  "access", "after", "alias", "all", "architecture", "array", "assert", "attribute",
  "begin", "block", "body", "buffer", "bus", "case", "component", "configuration",
  "constant", "disconnect", "downto", "else", "elsif", "end", "entity", "exit",
  "file", "for", "function", "generate", "generic", "group", "guarded", "if",
  "impure", "in", "inertial", "inout", "is", "label", "library", "linkage",
  "literal", "loop", "map", "mod", "nand", "new", "next", "nor",            
  "not", "null", "of", "on", "open", "or", "others", "out",
  "package", "port", "postponed", "procedure", "process", "pure", "range", "record",
  "reject", "rem", "report", "return", "rol", "ror", "select", "severity",
  "signal", "shared", "sla", "sll", "sra", "srl", "subtype", "then",
  "to", "transport", "type", "unaffected", "units", "until", "use", "variable",
  "wait", "when", "while", "with", "xnor", "xor"
};
    
static const std::unordered_set<std::string> IEEE_1076_VHDL_OPERATORS = {
  "+", "-", "*", "/", "&", "=", "/=", "<", "<=", ">", ">=",
  "=>", "**", ":=", "mod", "rem", "and", "or", "nand", "nor",
  "xor", "xnor", "not", "rol", "ror", "sla", "sll", "sra", "srl",
  "in", "not in", "abs", "??"
};
    
static const std::unordered_set<std::string> IEEE_1076_VHDL_SYMBOLS = {
  "(", ")", ",", ".", ":", ";", "'", "[", "]"
};

class Token {
public:
  explicit Token(TokenType type, const std::string& str, int line, int col) {
    this->type  = type;
    this->value = str;
    this->line  = line;
    this->col   = col;
    std::transform(this->value.begin(), this->value.end(), this->value.begin(), ::tolower);
  }
  explicit Token(const std::string& str, int line, int col){
    this->value = str;
    this->line  = line;
    this->col   = col;
    std::transform(this->value.begin(), this->value.end(), this->value.begin(), ::tolower);

    // Determine token type
    if (str.empty()) {
      type = TokenType::EoF;
    } else if (str == "\n") {
      type = TokenType::EoL;
    } else if (isKeyword(str)) {
      type = TokenType::Keyword;
    } else if (isOperator(str)) {
      type = TokenType::Operator;
    } else if (isSymbol(str)) {
      type = TokenType::Symbol;
    } else if (isIdentifier(str)) {
      type = TokenType::Identifier;
    } else {
      type = TokenType::Literal;
    }
  }

  static bool isKeyword(const std::string str) {
    return IEEE_1076_VHDL_KEYWORDS.count(str);
  }
  
  static bool isOperator(const std::string str) {
    return IEEE_1076_VHDL_OPERATORS.count(str);
  }

  static bool isSymbol(const std::string str) {
    return IEEE_1076_VHDL_SYMBOLS.count(str);
  }

  static bool isIdentifier(const std::string str) {
    if (str.empty() || !std::isalpha(str[0])) return false;
    return std::all_of(str.begin(), str.end(), [](char c) {
      return std::isalnum(c) || c == '_';
    });
  }

  TokenType getTokenType() {
    return this->type;
  }

  int getCol() {
    return this->col;
  }

  int getLine() {
    return this->line;
  }

  std::string getValue() {
    return this->value;
  }

  std::string toString() const {
    return (this->value != "\n" ? this->value : "\\n");
  }

  std::string toDebugString() const {
    std::ostringstream oss;
    oss << std::left << std::setw(10) << ((tokenTypeToString(type) == "\n") ? "\\n" : tokenTypeToString(type))
        << " \"" << this->toString() << "\""
        << "  at " << line << ":" << col;
    return oss.str();
  }


private:
  TokenType type;
  std::string value;
  int line;
  int col;
};