#ifndef __TYPE_HPP___
#define __TYPE_HPP___
#pragma once

#include <iostream>
#include <vector>

enum TypeBlock {TYPE_int, TYPE_bool, TYPE_char, TYPE_array, TYPE_list, TYPE_function, TYPE_void, TYPE_any};
void yyerror(const char *msg, ...);

class TonyType {
public:
    TonyType(TypeBlock current, TonyType *nested): current_type(current), nested_type(nested) {}
    TonyType(TypeBlock current, TonyType *nested, TonyType *ret, std::vector<TonyType *> args, bool dec): 
    current_type(current), nested_type(nested), returnType(ret), function_args(args), declDef(dec){    }
    ~TonyType() {delete nested_type;};
    TypeBlock get_current_type() {
        return current_type;
    }
    TonyType* get_nested_type() {
        return nested_type;
    }

    TonyType *get_return_type() {
        if (current_type != TYPE_function) yyerror("No return type for a non function type.");
        return returnType;
    }

    std::vector<TonyType *> get_function_args (){
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
    TonyType* nested_type;
    
    //For functions only
    TonyType *returnType;
    std::vector<TonyType *> function_args;
    bool declDef;
};

bool check_type_equality(TonyType* type1, TonyType* type2);

inline std::ostream& operator<< (std::ostream &out, TonyType* t) {
  TonyType* curr = t;
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