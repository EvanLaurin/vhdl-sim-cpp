#include "Parser.h"

Parser::Parser(const std::vector<Token>& tokens) {;
  this->tokens = tokens;
  this->current = 0;
}

Token Parser::peek() const{
  if (current >= tokens.size()) {
    return Token(TokenType::EoF, "", 0, 0);
  }
  return tokens[current];
}

Token Parser::advance() {
  while (current + 1 < tokens.size() && tokens[current + 1].getTokenType() == TokenType::EoL) {
    current++;
  }

  return tokens[current++];
}

bool Parser::match(TokenType type) {
  if (check(type)) {
    advance();
    return true;
  }
  return false;
}

bool Parser::matchKeyword (const std::string& value) {
  if (check(TokenType::Keyword) && peek().getValue() == value) {
    advance();
    return true;
  }
  return false;
}

bool Parser::matchSymbol (const std::string& value) {
  if (check(TokenType::Symbol) && peek().getValue() == value) {
    advance();
    return true;
  }
  return false;
}

bool  Parser::check(TokenType type) const {
  return peek().getTokenType() == type;
}

void Parser::expect(TokenType type, const std::string& error_message) {
  if (!match(type)) {
    throw std::runtime_error(error_message + " at line " + 
          std::to_string(peek().getLine()) + ":" + std::to_string(peek().getCol()) + " instead got : " + peek().getValue());
  }
}

void Parser::expectKeyword(const std::string& value, const std::string& error_message) {
  if (peek().getTokenType() != TokenType::Keyword || peek().getValue() != value) {
    throw std::runtime_error(error_message + " at line " +
          std::to_string(peek().getLine()) + ":" + std::to_string(peek().getCol()) +
          " - got '" + peek().getValue() + "'");
  }
  advance();
}

void Parser::expectSymbol(const std::string &value, const std::string &error_message) {
  if (peek().getTokenType() != TokenType::Symbol || peek().getValue() != value) {
    throw std::runtime_error(error_message + " at line " +
          std::to_string(peek().getLine()) + ":" + std::to_string(peek().getCol()) +
          " - got '" + peek().getValue() + "'");
  }
  advance();
}

VhdlFile& Parser::getTree() {
  return root;
}

void Parser::parse() {
  current = 0;
  auto tree = parse_vhdl_file();
  root = std::move(*tree);
}

std::unique_ptr<class VhdlFile> Parser::parse_vhdl_file() {
  auto file = std::make_unique<VhdlFile>();
  while (peek().getTokenType() != TokenType::EoF) {
    if (peek().getValue() == "entity") {
      auto entity_decl = parse_entity_declaration();
      file->setEntity(std::move(entity_decl));
    } else 
    if (peek().getValue() == "architecture") {
      auto archtc_decl = parse_architecture_declaration();
      file->setArchtc(std::move(archtc_decl));
    } else {
      // other -- ignore for now
      advance();
    }
  }
  return file;
}

std::unique_ptr<class EntityDeclaration> Parser::parse_entity_declaration() {
  auto entity_decl = std::make_unique<EntityDeclaration>();

  // entity
  expectKeyword("entity", "Expected 'entity' keyword");

  // <identifier>
  entity_decl->setIdentifier(peek().getValue());
  expect(TokenType::Identifier, "Expected entity name");

  // is
  expectKeyword("is", "Expected 'is' keyword");

  // <entity_header>
  entity_decl->setEntityHeader(parse_entity_header());

  // <entity_declarative_part> -- add later
  // [ begin <entity_statement_part> ] -- add later

  // end
  expectKeyword("end", "Expected 'end' keyword");

  // [ entity ]
  matchKeyword("entity");
  
  // [ <entity_simple_name> ]
  if (check(TokenType::Identifier)) {
    entity_decl->setSimpleName(peek().getValue());
    advance();
  }
  
  // ;
  expectSymbol(";", "Expected ';' symbol");

  return entity_decl;
}


std::unique_ptr<class ArchitectureDeclaration> Parser::parse_architecture_declaration() {
  auto archtc_decl = std::make_unique<ArchitectureDeclaration>();

  // architecture
  expectKeyword("architecture", "Expected 'architecture' keyword");

  // <identifier>
  archtc_decl->setIdentifier(peek().getValue());
  expect(TokenType::Identifier, "Expected architecture name");

    // is
  expectKeyword("is", "Expected 'is' keyword");

  return archtc_decl;
}


std::unique_ptr<class EntityHeader> Parser::parse_entity_header() {
  auto entity_head = std::make_unique<EntityHeader>();

  // [ <formal_generic_clause> ]
  if (check(TokenType::Keyword) && peek().getValue() == "generic") {
    // generic
    advance();

    // ( 
    expectSymbol("(", "Expected '(' symbol");

    // <generic_list> -- add later

  }
  
  // [ <formal_port_clause> ]
  if (check(TokenType::Keyword) && peek().getValue() == "port") {
    // port
    advance();

    // ( 
    expectSymbol("(", "Expected '(' symbol");

    // <Port_list>
    entity_head->setPortList(parse_interface_list());

    // )
    expectSymbol(")", "Expected ')' after port list");

    // ;
    expectSymbol(";", "Expected ';' after port clause");
  }

  if(peek().getTokenType() == TokenType::EoL) {advance();}
  return entity_head;
}


std::unique_ptr<InterfaceList> Parser::parse_interface_list() {
  auto intr_list = std::make_unique<InterfaceList>();

  bool expect_more = true;
  while (expect_more && !(check(TokenType::Symbol) && peek().getValue() == ")")) {
    intr_list->pushElement(parse_interface_element());

    if (check(TokenType::Symbol) && peek().getValue() == ";") {
      advance();

      // Check if this is a trailing semicolon (i.e., next is ")")
      if (check(TokenType::Symbol) && peek().getValue() == ")") {
        expect_more = false; // we're done
      }
    } else {
      expect_more = false;
    }
  }
  
  return intr_list;
}

std::unique_ptr<class InterfaceElement> Parser::parse_interface_element() {
  auto elem = std::make_unique<InterfaceElement>();

  // [ variable ], [ signal ], [ constant ]
  if (check(TokenType::Keyword)) {
    std::string tk_str = peek().getValue();
    if (tk_str == "constant" || tk_str == "signal" || tk_str == "variable") 
      advance();
  }

  // <identifier_list>
  elem->setIdentifier(peek().getValue());
  expect(TokenType::Identifier, "Expected identifier for interface element");

  // :
  expectSymbol(":", "Expected ':' symbol");

  // [ <mode> ]
  if (check(TokenType::Keyword)) {
    std::string tk_str = peek().getValue();
    if (tk_str == "in" || tk_str == "out" || tk_str == "inout"|| tk_str == "buffer"|| tk_str == "linkage") { 
      elem->setMode(peek().getValue());
      advance();
    }
  }
  elem->setType(parse_interface_type());

  return elem;
}


std::unique_ptr<class InterfaceType> Parser::parse_interface_type() {
  auto intr_type = std::make_unique<InterfaceType>();

  std::string str = peek().getValue();
  intr_type->setIdentifier(str);
  expect(TokenType::Identifier, "Expected type name");

  if (str == "bit_vector") {
    // (
    expectSymbol("(", "Expected '(' symbol");
            
    // Upper bound
    intr_type->setUpper(peek().getValue());
    expect(TokenType::Literal, "Expected upper bound");
            
    // downto, to
    intr_type->setDirection(peek().getValue());
    expect(TokenType::Keyword, "Expected 'downto' or 'to'");
            
    // Lower bound
    intr_type->setLower(peek().getValue());
    expect(TokenType::Literal, "Expected lower bound");
            
    // )
    expectSymbol(")", "Expected ')' symbol");
  } 

  return intr_type;
}