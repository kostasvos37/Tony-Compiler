#ifndef __SYMBOL_HPP___
#define __SYMBOL_HPP___
#pragma once

#include <vector>
#include <map>
#include <iostream>
#include "type.hpp"
#include <string>

void yyerror(const char *msg, ...);

struct SymbolEntry {
    TonyType* type;
    SymbolEntry() {}
    SymbolEntry(TonyType* t) : type(t) {};
};

class Scope {
public:
    Scope(TonyType *t) : locals(), size(0), returnType(t), hasReturn(false) {}
    SymbolEntry *lookup(std::string c) {
        if (locals.find(c) == locals.end()) {return nullptr;}
        return &locals[c];
    }
        
    void insert(std::string c, TonyType* t){
        if (locals.find(c) != locals.end()) yyerror("Variable already declared");
        locals[c] = SymbolEntry(t);
        ++size;
    }
    int getSize() const { return size; }
    TonyType *getReturnType() { return returnType;}

    //For testing
    void printScope(int ident) { 
        for (auto i = locals.begin(); i != locals.end(); ++i){
            std::cout << std::string(ident*4, ' ') << i->first << " " << i->second.type << std::endl;
        }  
    }
    void setHasReturn(){
        hasReturn = true;
    }
    bool getHasReturn(){
        return hasReturn;
    }
private:
    std::map<std::string, SymbolEntry> locals;
    int size;
    TonyType *returnType;
    bool hasReturn;
};

class SymbolTable{
public: 
    SymbolEntry *lookup(std::string c) {
        for (auto i = scopes.rbegin(); i!= scopes.rend(); i++){
            SymbolEntry *e = i->lookup(c);
            if (e!= nullptr) return e;
        }
        return nullptr;
    }

    SymbolEntry *lookupCurentScope(std::string c) {
        SymbolEntry *e = scopes.back().lookup(c);
        if (e!= nullptr) return e;
        return nullptr;
    }

    void insert(std::string c, TonyType* t){
        scopes.back().insert(c, t);
    }
    // This probably doesn't belong here, but works for now

    bool hasParentScope(){
        return scopes.size() > 1;
    }
    void insertIntoParentScope(std::string c, TonyType* t){
        scopes.at(scopes.size() -2).insert(c, t);
    }
    SymbolEntry * lookupParentScope(std::string c){
        if(hasParentScope()){
            SymbolEntry *e = scopes.at(scopes.size() -2).lookup(c);
            if (e!= nullptr) return e;
        }
        return nullptr;
    }
    void openScope(TonyType *t){
        scopes.push_back(Scope(t));
    }
    void closeScope(){
        scopes.pop_back();
    }
    int getSizeOfCurrentScope() const {
        return scopes.back().getSize();
    }

    TonyType *getCurrentScopeReturnType (){
        return scopes.back().getReturnType();
    }

    void setScopeHasReturn(){
        scopes.back().setHasReturn();
    }

    bool getScopeHasReturn(){
        return scopes.back().getHasReturn();
    }

    //For testing
    void printSymbolTable(){
        std::cout << "Printing symbol table" << std::endl;
        int scopecounter=0;
        for (auto i = scopes.begin(); i!= scopes.end(); i++){
            
            std::cout << std::string(scopecounter*4, ' ') << "<Scope " << scopecounter << ">" <<std::endl;
            //SymbolEntry *e = i->lookup(c);
            i->printScope(scopecounter);
            std::cout << std::string(scopecounter*4, ' ') << "</Scope " << scopecounter << ">" <<std::endl;
            scopecounter++;
        }

    }
private:
    std::vector<Scope> scopes;
};

extern SymbolTable st;

#endif