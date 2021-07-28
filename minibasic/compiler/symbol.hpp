#ifndef __SYMBOL_HPP__
#define __SYMBOL_HPP__

#include <vector>
#include <map>

void yyerror(const char *msg);

enum Type { TYPE_int, TYPE_bool };

struct SymbolEntry {
  Type type;
  int offset;
  SymbolEntry() {}
  SymbolEntry(Type t, int ofs) : type(t), offset(ofs) {}
};

class Scope {
public:
  Scope(int ofs = -1) : locals(), offset(ofs), size(0) {}
  SymbolEntry * lookup(char c) {
    if (locals.find(c) == locals.end()) return nullptr;
    return &locals[c];
  }
  void insert(char c, Type t) {
    if (locals.find(c) != locals.end()) yyerror("Duplicate variable");
    locals[c] = SymbolEntry(t, offset++);
    ++size;
  }
  int getSize() const { return size; }
  int getOffset() const { return offset; }
private:
  std::map<char, SymbolEntry> locals;
  int offset;
  int size;
};

class SymbolTable {
public:
  void openScope() {
    int ofs = scopes.empty() ? 0 : scopes.back().getOffset();
    scopes.push_back(Scope(ofs));
  }
  void closeScope() {
    scopes.pop_back();
  }
  SymbolEntry * lookup(char c) {
    for (auto i = scopes.rbegin(); i != scopes.rend(); ++i) {
      SymbolEntry *e = i->lookup(c);
      if (e != nullptr) return e;
    }
    yyerror("Variable not found");
    return nullptr;
  }
  void insert(char c, Type t) {
    scopes.back().insert(c, t);
  }
  int getSizeOfCurrentScope() const {
    return scopes.back().getSize();
  }
private:
  std::vector<Scope> scopes;
};

extern SymbolTable st;

#endif