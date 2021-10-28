#include "../ast.hpp"

/*================================================================================================*/
/*                                            AST                                                 */
/*================================================================================================*/

void AST::setLineno(int n){
    lineno = n;
}

/*================================================================================================*/
/*                                            Id                                                  */
/*================================================================================================*/

Id::Id(std::string v): var(v) {}
Id::~Id() {}
void Id::printOn(std::ostream &out) {
    out << "<Id name=\"" << var << "\"> ";
}

/*================================================================================================*/
/*                                        ArrayElement                                            */
/*================================================================================================*/

ArrayElement::ArrayElement(Atom *a, Expr *e): atom(a), expr(e) {
    pass_by_value=true;
}
ArrayElement::~ArrayElement() {
    delete atom; 
    delete expr;
}

void ArrayElement::printOn(std::ostream &out) {
    out << "\n<ArrayElement>\n" << *atom << "\n" << *expr << "\n</ArrayElement>\n";
}

/*================================================================================================*/
/*                                        StringLiteral                                           */
/*================================================================================================*/

StringLiteral::StringLiteral(std::string str) {
    int len = str.length();
    strlit = str;
    int i = 0; // Iterates over `str`.
    int j = 0; // Iterates over `strlit`.
    
    // We validate the escape characters (e.g: '\n', \0' etc...) in the string.
    // Initially they are parsed as two separate characters, so we must
    // squeeze them into a single character.
    while(i < len) {
      if(str[i] == '\\') {
        if(i == len-1) {
          yyerror("Invalid backslash at the end of string literal.");
        }

        // In this case, we've caught an escape character.
        switch(str[i+1]) {
          case 'n':  strlit[j] = '\n'; i+=2; break;
          case 't':  strlit[j] = '\t'; i+=2; break;
          case 'r':  strlit[j] = '\r'; i+=2; break;
          case '0':  strlit[j] = '\0'; i+=2; break;
          case '\\': strlit[j] = '\\'; i+=2; break;
          case '\'': strlit[j] = '\''; i+=2; break;
          case '\"': strlit[j] = '\"'; i+=2; break;
          case 'x':  strlit[j] = char(stoi(str.substr(i+2, 2), 0, 16)); i+=4; break;
        default:
          yyerror("Invalid escape character in string literal.");
        }
      } else {
        // In this case, we've caught a regular character.
        strlit[j] = str[i];
        i++;
      }
      j++;
    }

    // We throw away the trailing unused characters.
    strlit = strlit.substr(0, j);
  }
StringLiteral::~StringLiteral () {}

void StringLiteral::printOn(std::ostream &out) {
    out << "<String value=\"" << strlit << "\"> ";
  }

/*================================================================================================*/
/*                                        CharConst                                               */
/*================================================================================================*/

CharConst::CharConst(unsigned char c): char_const(c) {}
CharConst::~CharConst() {}

void CharConst::printOn(std::ostream &out) {
    out << "<CharConst value='"<< char_const << "\' ascii="<< (int) char_const << "> ";
}

/*================================================================================================*/
/*                                        IntConst                                                */
/*================================================================================================*/

IntConst::IntConst(int n): num(n) {}
IntConst::~IntConst() {}

void IntConst::printOn(std::ostream &out) {
    out << "<IntConst value=" << num << "> ";
}

/*================================================================================================*/
/*                                          New                                                   */
/*================================================================================================*/

New::New(TonyType *t, Expr *right): type_of_elems(t), expr(right) {}
New::~New() {
    delete type_of_elems; 
    delete expr;
}

void New::printOn(std::ostream &out)  {
    out << "<New> " << type_of_elems << *expr << "</New> ";
}

/*================================================================================================*/
/*                                          Nil                                                   */
/*================================================================================================*/

Nil::Nil() {}
Nil::~Nil() {}

void Nil::printOn(std::ostream &out)  {
    out << "<Nil> ";
}

/*================================================================================================*/
/*                                         Boolean                                                */
/*================================================================================================*/

Boolean::Boolean(std::string b1): boolean_value(b1) {
    if (b1=="true") b=true;
    else b=false;
  }
Boolean::~Boolean() {}

void Boolean::printOn(std::ostream &out)  {
    out << "<Boolean value=" << boolean_value << "> ";  
}

/*================================================================================================*/
/*                                          BinOp                                                 */
/*================================================================================================*/

BinOp::BinOp(Expr *l, std::string o, Expr *r): left(l), op(o), right(r) {}
BinOp::~BinOp() { 
    delete left; 
    delete right; 
}

void BinOp::printOn(std::ostream &out)  {
    out << "\n<Binop op=\"" << op << "\">\n" << *left << *right << "\n</BinOp>\n";
}

/*================================================================================================*/
/*                                           UnOp                                                 */
/*================================================================================================*/

UnOp::UnOp(std::string(o), Expr *r): op(o), right(r) {}
UnOp::~UnOp() { 
    delete right; 
}

void UnOp::printOn(std::ostream &out)  {
    out << "\n<UnOp op=\"" << op << "\">\n" << *right << "\n</UnOp>\n";
}

/*================================================================================================*/
/*                                           VarList                                              */
/*================================================================================================*/

VarList::VarList() {}
VarList::~VarList() {
    for (Id * i : ids) delete i;
}

void VarList::append(Id * id) {
    ids.push_back(id);
}

void VarList::reverse() {
    std::reverse(ids.begin(), ids.end());
}

void VarList::printOn(std::ostream &out)  {
    out << "\n<VarList>\n";
    out << type;
    for (Id * i : ids) {
      out << *i;
    }
    out << "\n</VarList>\n";
}

/*================================================================================================*/
/*                                           Formal                                               */
/*================================================================================================*/

Formal::Formal(VarList* v, bool i): var_list(v), is_ref(i) {}
Formal::~Formal() {
    delete var_list;
}

void Formal::printOn(std::ostream &out)  {
    out << "\n<Formal isRef=\"" << (is_ref ? "yes" : "no") << "\">\n" << *var_list << "</Formal>";
}

/*================================================================================================*/
/*                                         FormalList                                             */
/*================================================================================================*/

FormalList::FormalList() {}
FormalList::~FormalList() {
    for (Formal * f : formals) delete f;
}

void FormalList::append(Formal * f) {
    formals.push_back(f);
}

void FormalList::reverse(){
    std::reverse(formals.begin(), formals.end());
}

void FormalList::printOn(std::ostream &out)  {
    out << "\n<FormalList>\n";

    for (Formal * f : formals) {
        out << *f;
    }
    out << "\n</FormalList>\n";
}

/*================================================================================================*/
/*                                           Header                                               */
/*================================================================================================*/

Header::Header(TonyType *t, Id *name, FormalList *f): type(t), formals(f), id(name), isTyped(true) {}
Header::Header(Id *name, FormalList *f): formals(f), id(name), isTyped(false) {
    type = new TonyType(TYPE_void, nullptr);
}
Header::~Header(){ 
    delete formals; delete id;
}

void Header::printOn(std::ostream &out)  {
    out << "<Header>\n"; 
    if(!isTyped) {
      out << "<NoType>";
    } else {
      out << type;
    }
    out << *id; 
    if(!formals) {
      out << "<NoFormalList>";
    } else {
      out << *formals;
    }
    out << "\n</Header>\n";
}

/*================================================================================================*/
/*                                           Return                                               */
/*================================================================================================*/

Return::Return(Expr* e): ret_expr(e) {}
Return::~Return() {
    delete ret_expr;
}

void Return::printOn(std::ostream &out)  {
    out << "\n<Return>\n" << *ret_expr << "\n</Return>\n";
}

/*================================================================================================*/
/*                                           Exit                                                 */
/*================================================================================================*/

Exit::Exit() {}
Exit::~Exit() {}

void Exit::printOn(std::ostream &out)  {
    out << "\n<Exit>\n";
}

/*================================================================================================*/
/*                                         StmtBody                                               */
/*================================================================================================*/  

StmtBody::StmtBody(): stmts(), has_return(false), has_exit(false) {}
StmtBody::~StmtBody() {
    for (Stmt *s : stmts) delete s;
}

void StmtBody::append(Stmt* stmt) {
    stmts.push_back(stmt);
}

void StmtBody::reverse(){
    std::reverse(stmts.begin(), stmts.end());
}

void StmtBody::printOn(std::ostream &out)  {
    out << "\n<StmtBody>\n";
    for (Stmt *s : stmts) {
      out << *s;
    }
    out << "\n</StmtBody>\n";
}

/*================================================================================================*/
/*                                          Assign                                                */
/*================================================================================================*/ 

Assign::Assign(Atom *a, Expr *e): atom(a), expr(e) {}
Assign::~Assign() {
    delete atom; 
    delete expr;
}

void Assign::printOn(std::ostream &out)  {
    out << "\n<Assign>\n" << *atom << *expr << "\n</Assign>\n";
}

/*================================================================================================*/
/*                                           Skip                                                 */
/*================================================================================================*/ 

Skip::Skip() {}
Skip::~Skip() {}

void Skip::printOn(std::ostream &out)  {
    out << "\n<Skip>\n";
}

/*================================================================================================*/
/*                                            If                                                  */
/*================================================================================================*/ 

If::If(Expr* if_condition, StmtBody* if_stmt_body, If* next):
    condition(if_condition) ,stmt_body(if_stmt_body), next_if(next) {}

If::~If() {
    delete condition;
    delete stmt_body; 
    if(next_if != nullptr) delete next_if;
}

void If::printOn(std::ostream &out)  {
    out << "\n<If>\n"; 
    if (condition != nullptr) out << *condition;
    out << *stmt_body;
    if (next_if != nullptr) out << *next_if;
    out << "\n</If>\n";
}

/*================================================================================================*/
/*                                        SimpleList                                              */
/*================================================================================================*/

SimpleList::SimpleList(): simples() {}
SimpleList::~SimpleList() { 
    for (Simple *s : simples) delete s; 
}

void SimpleList::append(Simple *s) { 
    simples.push_back(s); 
}

void SimpleList::reverse(){
    std::reverse(simples.begin(), simples.end());
}

void SimpleList::printOn(std::ostream &out)  {
    out << "\n<SimplesList>\n";
    for (Simple *s : simples) {
        out << *s;
    }
    out << "\n</SimpleList>\n";
}

/*================================================================================================*/
/*                                            For                                                 */
/*================================================================================================*/

For::For(SimpleList *sl1, Expr *e, SimpleList *sl2, StmtBody *sb):
    initializations(sl1), condition(e), steps(sl2), stmt_body(sb) {}
For::~For() {
    delete initializations;
    delete condition;
    delete steps;
    delete stmt_body;
}

void For::printOn(std::ostream &out)  {
    out << "\n<For>\n" << *initializations << *condition << *steps  << *stmt_body << "\n</For>\n";
}

/*================================================================================================*/
/*                                          ExprList                                              */
/*================================================================================================*/ 

ExprList::ExprList(): expressions() {}
ExprList::~ExprList() { 
    for (Expr *e : expressions) delete e; 
}

void ExprList::append(Expr *e) { 
    expressions.push_back(e); 
}

void ExprList::reverse(){
    std::reverse(expressions.begin(), expressions.end());
}

void ExprList::printOn(std::ostream &out)  {
    out << "\n<ExprList>\n";
    for (Expr *e : expressions) {
      out << *e;
    }
    out << "\n</ExprList>\n";
}

/*================================================================================================*/
/*                                        FunctionCall                                            */
/*================================================================================================*/ 

FunctionCall::FunctionCall(Id *n): name(n), hasParams(false) {}
FunctionCall::FunctionCall(Id *n, ExprList *el): name(n), params(el), hasParams(true) {}
FunctionCall::~FunctionCall() {
    delete name; 
    if (hasParams) 
        delete params;
}

void FunctionCall::printOn(std::ostream &out)  {
    if(!hasParams)
        out << "\n<FunctionCall>\n" << *name << "\n</FunctionCall>\n";
    else
        out << "\n<FunctionCall>\n" << *name << *params << "\n</FunctionCall>\n";
}

/*================================================================================================*/
/*                                     FunctionDeclaration                                        */
/*================================================================================================*/

FunctionDeclaration::FunctionDeclaration(Header *hd): header(hd){}
FunctionDeclaration::~FunctionDeclaration() {
    delete header;
}

void FunctionDeclaration::printOn(std::ostream &out)  {
    out << "\n<FunctionDeclaration>\n" << *header << "\n</FunctionDeclaration>\n" ;
}

/*================================================================================================*/
/*                                     FunctionDefinition                                         */
/*================================================================================================*/

FunctionDefinition::FunctionDefinition(): header(), body(){}
FunctionDefinition::~FunctionDefinition() {
    delete header; delete body;
    for (AST *a : local_definitions) delete a;
}

void FunctionDefinition::append(AST* a) {
    local_definitions.push_back(a);
}

void FunctionDefinition::merge(Header *hd, StmtBody *st) {
    header = hd;
    body = st;
}

void FunctionDefinition::reverse(){
    std::reverse(local_definitions.begin(), local_definitions.end());
}

void FunctionDefinition::printOn(std::ostream &out)  {
    out << "\n<FunctionDefinition>\n" << *header; 
    for (AST *a : local_definitions) out << *a;
    out << *body << "\n</FunctionDefinition>\n" ;
}
