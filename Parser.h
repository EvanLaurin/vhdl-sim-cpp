#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <stdexcept>
#include "Token.h"
#include "Node.h"


class Parser {
public:
  Parser(const std::vector<Token>& tokens);
  void parse(); 

  VhdlFile& getTree();

private:
  std::vector<Token> tokens;
  size_t current;

  VhdlFile root;


  // Utility functions
  Token peek() const;
  Token advance();
  bool match(TokenType type);
  bool matchKeyword(const std::string &value);
  bool matchSymbol(const std::string &value);
  bool check(TokenType type) const;
  void expect(TokenType type, const std::string& error_message);
  void expectKeyword(const std::string &value, const std::string &error_message);
  void expectSymbol(const std::string &value, const std::string &error_message);

  // Recursive-descent parsing functions
  std::unique_ptr<class VhdlFile> parse_vhdl_file();
  std::unique_ptr<class EntityDeclaration> parse_entity_declaration();
  std::unique_ptr<class EntityHeader> parse_entity_header();
  std::unique_ptr<class InterfaceList> parse_interface_list();
  std::unique_ptr<class ArchitectureDeclaration>parse_architecture_declaration();
  void parse_entity_declarative_part();
  void parse_entity_statement_part();
  void parse_entity_statement();

  // Interface related functions
  std::unique_ptr<class InterfaceElement> parse_interface_element();
  void parse_interface_declaration();
  void parse_interface_object_declaration();
  void parse_interface_constant_declaration();
  void parse_interface_signal_declaration();
  void parse_interface_variable_declaration();
  void parse_interface_file_declaration();
  

  // Mode related functions
  void parse_mode_indication();
  void parse_simple_mode_indication();
  void parse_mode();
  void parse_mode_view_indication();
  void parse_record_mode_view_indication();
  void parse_array_mode_view_indication();

  // Type and expression related functions
  std::unique_ptr<class InterfaceType> parse_interface_type();
  void parse_subtype_indication();
  void parse_static_conditional_expression();
  void parse_signal_mode_indication();
  void parse_identifier_list();
};