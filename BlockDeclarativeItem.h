#include "Node.h"

/*
block_declarative_item ::=
  subprogram_declaration
| subprogram_body
| subprogram_instantiation_declaration
| package_declaration
| package_body
| package_instantiation_declaration
| type_declaration
| subtype_declaration
| mode_view_declaration
| constant_declaration
| signal_declaration
| shared_variable_declaration
| file_declaration
| alias_declaration
| component_declaration
| attribute_declaration
| attribute_specification
| configuration_specification
| disconnection_specification
| use_clause
| group_template_declaration
| group_declaration
| PSL_Property_Declaration
| PSL_Sequence_Declaration
| PSL_Clock_Declaration
*/


class BlockDeclarativeItem : public Node {
public:
  virtual ~BlockDeclarativeItem() = default; 
};


class ConstantDeclaration : public BlockDeclarativeItem {
public:
  std::string name;
  std::string value;

  std::string toString() const override {
    return "ConstantDeclaration(" + name + " := " + value + ")";
  }
};


class SignalDeclaration : public BlockDeclarativeItem {
public:
  std::string name;
  std::string type;

  std::string toString() const override {
    return "SignalDeclaration(" + name + ": " + type + ")";
  }
};

// component_declaration 

// type_declaration

// subtype_declaration 

// use_clause

// attribute_specification 
