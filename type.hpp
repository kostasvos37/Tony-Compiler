#ifndef __TYPE_HPP___
#define __TYPE_HPP___
#pragma once

#include <iostream>
#include <vector>

enum TypeBlock {TYPE_int, TYPE_bool, TYPE_char, TYPE_array, TYPE_list, TYPE_function, TYPE_void, TYPE_any};
void yyerror(const char *);

class Type {
public:
    Type(TypeBlock current, Type *nested): current_type(current), nested_type(nested) {}
    Type(TypeBlock current, Type *nested, Type *ret, std::vector<Type *> args, bool dec): 
    current_type(current), nested_type(nested), returnType(ret), function_args(args), declDef(dec){    }
    ~Type() {delete nested_type;};
    TypeBlock get_current_type() {
        return current_type;
    }
    Type* get_nested_type() {
        return nested_type;
    }

    Type *get_return_type() {
        if (current_type != TYPE_function) yyerror("No return type for a non function");
        return returnType;
    }

    std::vector<Type *> get_function_args (){
      return function_args;
    }
    bool isDeclared() {
      return declDef;
    }
    void toggleDeclDef(){
      declDef = !declDef;
    }

protected:
    TypeBlock current_type;
    Type* nested_type;
    
    //For functions only
    Type *returnType;
    std::vector<Type *> function_args;
    bool declDef;
};

bool check_type_equality(Type* type1, Type* type2);

inline std::ostream& operator<< (std::ostream &out, Type* t) {
  Type* curr = t;
  while (curr != nullptr) {
    switch (curr->get_current_type())
    {
      case TYPE_int: out << "\"int\""; break;
      case TYPE_bool: out << "\"bool\""; break;
      case TYPE_char: out << "\"char\""; break;
      case TYPE_array: out << "array: "; break;
      case TYPE_list: out << "list: "; break;
      case TYPE_function: {
        out << "function ->" << curr->get_return_type() << ", parameters (";
        for (auto i: curr->get_function_args()) out << i << ", ";
        out << ")";} 
      break;
      case TYPE_void: out << "void "; break;
      default: out << "\"invalid\""; break;
    }
    curr = curr->get_nested_type();
  }
  return out;
}

#endif