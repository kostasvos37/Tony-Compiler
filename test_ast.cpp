#include <iostream>
#include <string>
#include "ast.hpp"

void test (AST * item){
    std::cout << "Test cout \n";
    std::cout << *item << std::endl;
}

int main(){
    Type *t = new Type("int");
    Type *t2 = new Type("list", t);

    Id *id1 = new Id("var1");
    Id *id2 = new Id("stamou");
    StringLiteral *str = new StringLiteral("fwtakaros");

    IntConst *num = new IntConst(14);
    CharConst *cha = new CharConst('v');
    UnOp *un = new UnOp(std::string("+"), num);
    BinOp *bin = new BinOp(num, std::string("and"), id2);
    Boolean *b = new Boolean("true");
    New *ne = new New(t2, bin);
    Nil *nil = new Nil();
    Array *arr = new Array(id1, num);
    ExprList *exprlist = new ExprList();
    exprlist->append(id1);
    exprlist->append(cha);
    exprlist->append(nil);
    FunctionCall *call1 = new FunctionCall(id1);
    FunctionCall *call2 = new FunctionCall(id1, exprlist);

    VarList *vl = new VarList();
    vl->append(id1);
    vl->append(id2);
    vl->set_type(t);

    Formal *f1 = new Formal(vl, true);    
    Formal *f2 = new Formal(vl, false);
    FormalList *fl = new FormalList();
    fl->append(f1);
    fl->append(f2);

    Header *h1 = new Header (t, id1, NULL);
    Header *h2 = new Header (t, id1, fl);
    Header *h3 = new Header (NULL, id1, NULL);
    Header *h4 = new Header (NULL, id1, fl);

    //Simples
    Skip *skip = new Skip();
    Assign *ass = new Assign(id1, un);
    SimpleList *slist = new SimpleList();
    slist->append(skip);
    slist->append(ass);
    slist->append(call1);


    //Statements
    StmtBody *st1= new StmtBody();
    st1->append(skip);
    st1->append(ass);
    StmtBody *st2 = new StmtBody();

    For *fork = new For(slist, bin, slist, st1);
    Exit *exit = new Exit();
    Return *ret = new Return(un);

    Else *el_empty = new Else();
    Else *el_full = new Else(st1);

    Elsif *ei = new Elsif();
    Elsif *ei2 = new Elsif();
    ei->append(id1, st1);
    ei->append(id2, st1);

    If *if1 = new If(id1, st1, ei2, el_full);

    If *if2 = new If(id1, st2, ei, el_empty);

    FunctionDeclaration *fd = new FunctionDeclaration(h1);

    FunctionDefinitionList *fdl1 = new FunctionDefinitionList();
    fdl1->append(fd);
    fdl1->append(vl);

    FunctionDefinition *fd1 = new FunctionDefinition(h3, fdl1, st2);


    FunctionDefinitionList *fdl2 = new FunctionDefinitionList();
    fdl2->append(fd);
    fdl2->append(vl);
    fdl2->append(fd1);
    

    test(fd1);
}

