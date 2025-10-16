#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Lexer.h"
#include "Parser.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file.vhd>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << argv[1] << "\n";
    return 1;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();  // read entire file into buffer
  std::string input = buffer.str();


  // Lexing
  std::cout << "\n--- Lexing ---\n";

  std::vector<Token> tokens;
  Lexer lexer(input);

  int tokens_length = 0;
  while(lexer.hasMoreTokens()) {
    Token token = lexer.getNextToken();
    if (token.getTokenType() == TokenType::Error) {
      std::cout << token.getValue() << ", line: "<< token.getLine() << " ," <<token.getCol() <<'\n';
      return -1;
    }
    tokens.push_back(token);
    tokens_length++;
  }
  Token eof_tk = lexer.getNextToken();
  tokens.push_back(eof_tk);
  tokens_length++;

  for (int i = 0; i < tokens_length; i++) {
    std::cout << tokens.at(i).toDebugString() << "\n";
  }

  // Parsing
  std::cout << "\n--- Parsing ---\n";

  try {
    Parser parser(tokens);
    parser.parse();
    std::cout << "\nParsing completed successfully!\n";
    std::cout << "\n--- AST ---\n";
    std::cout << parser.getTree().toString() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Parsing error: " << e.what() << std::endl;
    return 1;
  }
}