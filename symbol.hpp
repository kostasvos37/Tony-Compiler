#ifndef __SYMBOL_HPP___
#define __SYMBOL_HPP___
#pragma once

#include <vector>
#include <map>

void yyerror(const char *msg);

enum Type {TYPE_int, TYPE_bool, TYPE_char, TYPE_array, TYPE_list};

struct SymbolEntry {
    Type type;
    SymbolEntry() {}
    SymbolEntry(Type t) : type(t) {};
};

class Scope {
public:
    Scope() : locals() {}
    SymbolEntry *lookup(std::string c) {
        if (locals.find(c) == locals.end()) return nullptr;
        return &locals[c];
        
    }
    void insert(std::string c, Type t){
        if (locals.find(c) != locals.end()) yyerror("Variable already declared");
        locals[c] = SymbolEntry(t);
    };
private:
    std::map<std::string, SymbolEntry> locals;
};

class SymbolTable{
public: 
    SymbolEntry *lookup(std::string c) {
        for (auto i = scopes.rbegin(); i!= scopes.rend(); i++){
            SymbolEntry *e = i->lookup(c);
            if (e!= nullptr) return e;
        }
        yyerror("Variable not found");
        return nullptr;
    }
    void insert(std::string c, Type t){
        scopes.back().insert(c, t);
    }
    void openScope(){
        scopes.push_back(Scope());
    }
    void closesScope(){
        scopes.pop_back();
    }
private:
    std::vector<Scope> scopes;
};

extern SymbolTable st;

#endif