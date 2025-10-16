#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <stdexcept>
#include "Token.h"
#include "BlockDeclarativeItem.h"

class Node {
public:
  virtual ~Node() = default;
  virtual std::string toString() const = 0;
};


class InterfaceType : public Node {
public:
  std::string identifier;
  std::string upper;
  std::string direction;
  std::string lower;

  void setIdentifier(const std::string& id) {
    this->identifier = id;
  }

  void setUpper(const std::string& upper) {
    this->upper = upper;
  }

  void setDirection(const std::string& direction) {
    this->direction = direction;
  }

  void setLower(const std::string& lower) {
    this->lower = lower;
  }

  std::string toString() const override {
    if (!direction.empty() && !upper.empty() && !lower.empty()) {
      return identifier + "(" + upper + " " + direction + " " + lower + ")";
    }
    return identifier;
  }
};


class InterfaceElement : public Node {
public:
  std::string identifier;
  std::string mode;   
  std::unique_ptr<class InterfaceType> type;

  void setIdentifier(const std::string& id) {
    this->identifier = id;
  }

  void setMode(const std::string& mode) {
    this->mode = mode;
  }

  void setType(std::unique_ptr<class InterfaceType> type) {
    this->type = std::move(type);
  }

  std::string toString() const override {
    return "InterfaceElement(" + identifier + ", " + mode + ")\n" + 
           (type ? type->toString() : "null");
  }
};


class InterfaceList : public Node {
public:
  std::vector<std::unique_ptr<class InterfaceElement>> elems;

  void pushElement(std::unique_ptr<class InterfaceElement> elem) {
    elems.push_back(std::move(elem));
  }

  std::string toString() const override {
    std::string result = "InterfaceList[" + std::to_string(elems.size()) + " elements]\n";
    for (const auto& elem : elems) {
      result += elem->toString() + "\n";
    }
    return result;
  }
};


class EntityHeader : public Node {
public:
  std::unique_ptr<class InterfaceList> port_list;
  std::unique_ptr<class InterfaceList> generic_list;

  void setPortList(std::unique_ptr<class InterfaceList> port_list) {
    this->port_list = std::move(port_list);
  }

  std::string toString() const {
    return std::string("EntityHeader\n") + 
           (port_list ? port_list->toString() : "null") + "\n" + 
           (generic_list ? generic_list->toString() : "null");
  }
};


class EntityDeclaration : public Node {
public:
  std::string identifier;
  std::string simple_name;
  std::unique_ptr<class EntityHeader> entity_header;

  void setIdentifier(const std::string& id) {
    this->identifier = id;
  }

  void setSimpleName(const std::string& name) {
    this->simple_name = name;
  }

  void setEntityHeader(std::unique_ptr<class EntityHeader> header) {
    this->entity_header = std::move(header);
  }

  std::string toString() const {
    return "EntityDeclaration(" + identifier + ")" + "\n" + entity_header->toString();
  }
};

class ArchitectureDeclaration : public Node {
public:
  std::string identifier;
  std::string simple_name;
  std::unique_ptr<class EntityHeader> archtct_decl_part;
  std::unique_ptr<class EntityHeader> entity_header;

  void setIdentifier(const std::string& id) {
    this->identifier = id;
  }

  void setSimpleName(const std::string& name) {
    this->simple_name = name;
  }

  std::string toString() const override {
    return "ArchitectureDeclaration(" + identifier + ", " + simple_name + ")";
  }
};

class ArchitectureDeclarativePart : public Node {
public:
  std::vector<std::unique_ptr<BlockDeclarativeItem>> items;

  void additem(std::unique_ptr<BlockDeclarativeItem> item) {
    items.push_back(std::move(item));
  }

  std::string toString() const override {
    std::string result = "ArchitectureDeclarativePart\n";
    for (const auto& item : items) {
      result += item->toString() + "\n";
    }
    return result;
  }
};


class VhdlFile : public Node {
public:
  std::string identifier;
  std::unique_ptr<class EntityDeclaration> entity;
  std::unique_ptr<class ArchitectureDeclaration> archtc;

  void setIdentifier(const std::string& id) {
    this->identifier = id;
  }

  void setEntity(std::unique_ptr<class EntityDeclaration> entity) {
    this->entity = std::move(entity);
  }

  void setArchtc(std::unique_ptr<class ArchitectureDeclaration> archtc) {
    this->archtc = std::move(archtc);
  }

  std::string toString() const {
    return "VhdlFile(" + identifier + ")" + "\n" + entity->toString();
  }
};
