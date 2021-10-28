#include <iostream>
#include "type.hpp"


TonyType::TonyType(TypeBlock current, TonyType *nested): current_type(current), nested_type(nested), pass(VAL) {}
TonyType::TonyType(TypeBlock current, TonyType *nested, TonyType *ret, std::vector<TonyType *> args, bool dec): 
current_type(current), nested_type(nested), returnType(ret), function_args(args), declDef(dec), pass(VAL){}
TonyType::~TonyType() {
    delete nested_type;
}

TypeBlock TonyType::get_current_type() {
    return current_type;
}
TonyType* TonyType::get_nested_type() {
    return nested_type;
}

TonyType *TonyType::get_return_type() {
    if (current_type != TYPE_function) yyerror("No return type for a non function type.");
    return returnType;
}

void TonyType::setPassMode(PassMode p){
    pass = p;
}
PassMode TonyType::getPassMode(){
    return pass;
}

int TonyType::get_array_size() {
    if (current_type != TYPE_array) yyerror("No array size for non-array type.");
    return size;
}

void TonyType::set_array_size(int n) {
    if (current_type != TYPE_array) yyerror("No array size for non-array type.");
    size = n;
}

    // TODO: Implement this for all types
std::string TonyType::toString() {
      switch(current_type) {
        case TYPE_int: return "int";
        case TYPE_char: return "char";
        case TYPE_bool: return "bool";
        case TYPE_array: return "array";
        case TYPE_list: return "list";
        case TYPE_any: return "any";
        case TYPE_function: return "function";
        case TYPE_void: return "void";
        default: return "unknown";
      }
}
    
int TonyType::get_data_size_of_type(){
      /* Return the size (in Bytes) of an object that has this type. */
      
      switch(current_type) {
        case TYPE_int: return 4;
        case TYPE_char: return 1;
        case TYPE_bool: return 1;
        case TYPE_array: return 8;
        case TYPE_list: return 8 + nested_type->get_data_size_of_type();
        case TYPE_any: return 4;
        default: return 0;
      }
}

    /*
     * Types have their own hash string key so that they can be found in the
     * `llvm_list_types` catalog (if they are created first).
     * 
     * Example: list [list [char]] : "list_list_char"
     */
std::string TonyType::createHashKeyForType() {
      switch(get_current_type()) {
        case TYPE_int: return std::string("int");
        case TYPE_bool: return std::string("bool");
        case TYPE_char: return std::string("char");
        case TYPE_any: return std::string("int");
        case TYPE_array: return std::string("arr");
        case TYPE_list: {
          return std::string("list_") + nested_type->createHashKeyForType();
        }
        default: yyerror("Cannot have that type in a list.");
      }
      return std::string("error");
}

std::vector<TonyType *> TonyType::get_function_args (){
      return function_args;
}

void TonyType::addPreviousScopeArg (std::string arg, TonyType *t){
      previous_scope_args[arg] = t;
}

std::map<std::string, TonyType*> TonyType::getPreviousScopeArgs (){
      return previous_scope_args;
}

bool TonyType::isDeclared() {
      return declDef;
}
void TonyType::toggleDeclDef(){
      declDef = !declDef;
}

bool check_type_equality(TonyType* type1, TonyType* type2) {
    if (type1->get_current_type() == TYPE_any || type2->get_current_type() == TYPE_any) {
        return true;
    }
    if (type1->get_current_type() != type2->get_current_type()) {
        return false;
    }
    if ((type1->get_nested_type() == nullptr && type2->get_nested_type() != nullptr) || 
        (type1->get_nested_type() != nullptr && type2->get_nested_type() == nullptr)) {
        return false;
    }

    // function type checks
    if(type1->get_current_type()==TYPE_function){
        if(!check_type_equality(type1->get_return_type(), type2->get_return_type()))
            return false;
        std::vector<TonyType *> args1 = type1->get_function_args();
        std::vector<TonyType *> args2 = type2->get_function_args();
        if(args1.size() != args2.size())
            return false;
        for(int i=0; i< (int) args1.size(); i++){
            if (!check_type_equality(args1[i], args2[i]))
                return false;
        }
        // all good until here, rest should be empty
    }

    if (type1->get_nested_type() == nullptr && type2->get_nested_type() == nullptr) {
        return true;
    }
    return check_type_equality(type1->get_nested_type(), type2->get_nested_type());
}

bool is_nil_constant(TonyType *type) {
    if (type->get_current_type() != TYPE_list) {
        return false;
    }
    if (type->get_nested_type() == nullptr) {
        return false;
    }
    if (type->get_nested_type()->get_current_type() == TYPE_any) {
        return true;
    }
    return false;
} 

