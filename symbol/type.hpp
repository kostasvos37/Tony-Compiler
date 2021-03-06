#ifndef __TYPE_HPP___
#define __TYPE_HPP___
#pragma once

#include <iostream>
#include <vector>
#include <map>

enum TypeBlock {TYPE_int, TYPE_bool, TYPE_char, TYPE_array, TYPE_list, TYPE_function, TYPE_void, TYPE_any};
enum PassMode {VAL, REF};
void yyerror(const char *msg, ...);

class TonyType {
public:
    TonyType(TypeBlock current, TonyType *nested);
    TonyType(TypeBlock current, TonyType *nested, TonyType *ret, std::vector<TonyType *> args, bool dec);
    ~TonyType();

    TypeBlock get_current_type() ;
    TonyType* get_nested_type();
    TonyType *get_return_type();

    void setPassMode(PassMode p);
    PassMode getPassMode();

    int get_array_size();
    void set_array_size(int n);

    std::string toString();
    
    int get_data_size_of_type();
    std::string createHashKeyForType();

    std::vector<TonyType *> get_function_args ();
    void addPreviousScopeArg (std::string arg, TonyType *t);
    std::map<std::string, TonyType*> getPreviousScopeArgs ();

    bool isDeclared();
    void toggleDeclDef();
protected:
    TypeBlock current_type;
    TonyType* nested_type;
    
    //For functions only
    TonyType *returnType;
    std::vector<TonyType *> function_args;
    std::map<std::string, TonyType*> previous_scope_args;
    bool declDef;

    //For arrays only
    int size;

    //Passmode
    PassMode pass;


};

bool check_type_equality(TonyType* type1, TonyType* type2);
bool is_nil_constant(TonyType *type);
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