#include <iostream>
#include "type.hpp"

using namespace std;

bool check_type_equality(Type* type1, Type* type2) {
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
    if (type1->get_nested_type() == nullptr && type2->get_nested_type() == nullptr) {
        return true;
    }
    return check_type_equality(type1->get_nested_type(), type2->get_nested_type());
}

bool is_nil_constant(Type *type) {
    if (type->get_current_type() != TYPE_list) {
        std::cout << "ATTENTION: You didn't provide a list.\n";
        return false;
    }
    if (type->get_nested_type() == nullptr) {
        std::cout << "ATTENTION: The list didn't have a nested type! (this shouldn't have happened)\n";
        return false;
    }
    if (type->get_nested_type()->get_current_type() == TYPE_any) {
        return true;
    }
    return false;
} 
