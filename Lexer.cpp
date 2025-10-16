#include "Lexer.h"
#include "Token.h"

Lexer::Lexer(const std::string& input) {
  this->input = input;
  this->pos   = 0;
  this->line  = 1;
  this->col   = 0;
  this->max_pos = input.size();
}

Token Lexer::getNextToken() {
  while (true) {
    skipWhitespace();      
    if (!skipComments()) {
      break;
    }                  
  }

  if (pos >= max_pos) {
    return Token(TokenType::EoF, "", line, col);
  }

  char cur = input[pos];
  int tok_col = col;
  int tok_line = line;
  std::string token_value = "";

  // identifiers / keywords / operators
  if (isalpha(cur)) {
    while (pos < max_pos && (isalnum(input[pos]) || input[pos] == '_')) {
      token_value += input[pos++];
      col++;
    }
    return Token(token_value, tok_line, tok_col);
  }

  // numeric / based literals
  if (isdigit(cur)) {
    while (pos < max_pos && isdigit(input[pos])) {
      token_value += input[pos++];
      col++;
    }
    if (pos < max_pos && input[pos] == '.') {
      token_value += input[pos++];
      col++;
      while (pos < max_pos && isdigit(input[pos])) {
        token_value += input[pos++];
        col++;
      }
    } else if (pos < max_pos && input[pos] == '#') {
      token_value += input[pos++];
      col++;
      while (pos < max_pos && input[pos] != '#') {
        char ch = tolower(input[pos]);
        if (!isalnum(ch) && ch != '.' && ch != '-' && ch != '+' && ch != 'e') {
          return Token(TokenType::Error, "Err: invalid character in based literal", tok_line, tok_col);
        }
        token_value += input[pos++];
        col++;
      }
      if (pos < max_pos && input[pos] == '#') {
        token_value += input[pos++];
        col++;
      } else {
        return Token(TokenType::Error, "Err: missing closing '#'", tok_line, tok_col);
      }
    }
    return Token(token_value, tok_line, tok_col);
  }

  // string literal
  if (cur == '"') {
    token_value += input[pos++];
    col++;
    while (pos < max_pos && input[pos] != '"') {
      token_value += input[pos++];
      col++;
    }
    if (pos < max_pos && input[pos] == '"') {
      token_value += input[pos++];
      col++;
    } else {
      return Token(TokenType::Error, "Err: missing closing quote", tok_line, tok_col);
    }
    return Token(token_value, tok_line, tok_col);
  }

  // character literal
  if (cur == '\'') {
    token_value += input[pos++];
    col++;
    if (pos < input.size() && isalnum(input[pos])) {
      token_value += input[pos++];
      col++;
    } else {
      return Token(TokenType::Error, "Err: invalid char literal", tok_line, tok_col);
    }
    if (pos < input.size() && input[pos] == '\'') {
      token_value += input[pos++];
      col++;
    } else {
      return Token(TokenType::Error, "Err: missing closing single quote", tok_line, tok_col);
    }
    return Token(token_value, tok_line, tok_col);
  }

  // end-of-line
  if (cur == '\n') {
    token_value += input[pos++];
    line++;
    col = 0;
    return Token(token_value, tok_line, tok_col);
  }

  // operators (including two-character operators)
  if (strchr("+-*/&=<>:?", input[pos])) {
    if (pos + 1 < input.size()) {
      std::string twoChar = std::string(1, input[pos]) + input[pos + 1];
      if (Token::isOperator(twoChar)) {
        pos += 2;
        col += 2;
        return Token(twoChar, tok_line, tok_col);
      }
    }

    std::string oneChar(1, input[pos]);
    if (Token::isOperator(oneChar)) {
      pos++;
      col++;
      return Token(oneChar, tok_line, tok_col);
    }
  }

  // symbols (punctuation and delimiters)
  std::string singleChar(1, input[pos]);
  if (Token::isSymbol(singleChar)) {
    token_value += input[pos++];
    col++;
    return Token(token_value, tok_line, tok_col);
  }
  
  // unknown character
  return Token(TokenType::Error, "Err: unknown token", tok_line, tok_col);
}

bool Lexer::hasMoreTokens() const {
  return pos < max_pos;
}

void Lexer::skipWhitespace() {
  while(pos < max_pos && (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\r')) { 
    pos++; 
    col++; 
  }
};

bool Lexer::skipComments() {
  if (pos + 1 >= input.size()) return false;
  if (input[pos] == '-' && input[pos + 1] == '-') {
    pos += 2;
    col += 2;
    while (pos < input.size() && input[pos] != '\n') {
      pos++;
      col++;
    }
    return true;
  }
  return false;
}