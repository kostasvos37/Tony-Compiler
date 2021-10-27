#include "ast.hpp"

llvm::LLVMContext AST::TheContext;
llvm::IRBuilder<> AST::Builder(TheContext);
llvm::GlobalVariable *AST::TheVars;
std::unique_ptr<llvm::Module> AST::TheModule;
std::unique_ptr<llvm::legacy::FunctionPassManager> AST::TheFPM;

llvm::Function *AST::TheMalloc;

llvm::Type *AST::i1;
llvm::Type *AST::i8;
llvm::Type *AST::i32;
llvm::Type *AST::i64;
llvm::Type *AST::voidT;

std::vector<RuntimeBlock *> AST::blocks;
RuntimeScope AST::scopes;

/*================================================================================================*/
/*                                            AST                                                 */
/*================================================================================================*/

void AST::llvm_compile_and_dump(bool optimize){
    // Initialize    
    TheModule = std::make_unique<llvm::Module>("tony program", TheContext);

    i1 = llvm::IntegerType::get(TheContext, 1);
    i8 = llvm::IntegerType::get(TheContext, 8);
    i32 = llvm::IntegerType::get(TheContext, 32);
    i64 = llvm::IntegerType::get(TheContext, 64);
    voidT = llvm::Type::getVoidTy(TheContext);

    // We'll use 'TheMalloc' to allocate memory blocks with LLVM.
    // This is needed currently for creating Tony lists.
    llvm::FunctionType *malloc_type = 
      llvm::FunctionType::get(llvm::PointerType::get(i8, 0),
                              {i32}, false);
    TheMalloc = 
      llvm::Function::Create(malloc_type, llvm::Function::ExternalLinkage,
                             "GC_malloc", TheModule.get());
    
    // Global Scope
    scopes.openRuntimeScope();
    initializeLibraryFunctions();
    scopes.openRuntimeScope();
    
    
    //Create "dummy main function" which calls the first program function
    llvm::FunctionType *main_type = llvm::FunctionType::get(i32, std::vector<llvm::Type *>{}, false);
    llvm::Function * Main = llvm::Function::Create(main_type, llvm::Function::ExternalLinkage, "main", TheModule.get());
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", Main);
 
    // Emit Program Code
    
    auto program = compile();
    auto *progFunc = TheModule->getFunction(program->getName());

    Builder.SetInsertPoint(BB);
    Builder.CreateCall(progFunc, {});
    Builder.CreateRet(c32(0));

    // Close Program Scope
    scopes.closeRuntimeScope();
    scopes.closeRuntimeScope();

    bool bad = llvm::verifyModule(*TheModule, &llvm::errs());
    if (bad){
      std::cerr << "The IR is bad!\n";
      TheModule->print(llvm::errs(), nullptr);
      std::exit(1);
    }

    if(optimize){
      optimizeModule();
    }

    std::error_code EC;

    llvm::raw_ostream *out2 = new llvm::raw_fd_ostream("out.ll", EC);
    TheModule->print(*out2, nullptr);
}

void AST::optimizeModule(){

    TheFPM = std::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());
    if (true) {
      TheFPM->add(llvm::createPromoteMemoryToRegisterPass());
      TheFPM->add(llvm::createInstructionCombiningPass());
      TheFPM->add(llvm::createReassociatePass());
      TheFPM->add(llvm::createGVNPass());
      TheFPM->add(llvm::createCFGSimplificationPass());
    }
    TheFPM->doInitialization();

    for (auto &F: *TheModule)
      TheFPM->run(F);

}

void AST::initializeLibraryFunctions(){
    // puti (int n)
    llvm::FunctionType *writeIntegerType = llvm::FunctionType::get(voidT, std::vector<llvm::Type *>{i32}, false);
	  scopes.insertFunc("puti",  llvm::Function::Create(writeIntegerType, llvm::Function::ExternalLinkage, "writeInteger", TheModule.get()));

    // putc (char c)
    llvm::FunctionType *writeCharType = llvm::FunctionType::get(voidT, std::vector<llvm::Type *>{i8}, false);
	  scopes.insertFunc("putc",  llvm::Function::Create(writeCharType, llvm::Function::ExternalLinkage, "writeChar", TheModule.get()));

    // putb (bool b)
    llvm::FunctionType *writeBooleanType = llvm::FunctionType::get(voidT, std::vector<llvm::Type *>{i1}, false);
	  scopes.insertFunc("putb",  llvm::Function::Create(writeBooleanType, llvm::Function::ExternalLinkage, "writeBoolean", TheModule.get()));

    // puts (char [])
    llvm::FunctionType *writeStringType = llvm::FunctionType::get(voidT, {llvm::PointerType::get(i8,0)}, false);
    scopes.insertFunc("puts", llvm::Function::Create(writeStringType, llvm::Function::ExternalLinkage, "writeString", TheModule.get()));
    // int geti ()
    llvm::FunctionType *getiType = llvm::FunctionType::get(i32, std::vector<llvm::Type *>{}, false);
	  scopes.insertFunc("geti",  llvm::Function::Create(getiType, llvm::Function::ExternalLinkage, "readInteger", TheModule.get()));
    
    // char getc()
    llvm::FunctionType *getcType = llvm::FunctionType::get(i8, std::vector<llvm::Type *>{}, false);
	  scopes.insertFunc("getc",  llvm::Function::Create(getcType, llvm::Function::ExternalLinkage, "readChar", TheModule.get()));
    
    // bool getb ()
    llvm::FunctionType *getbType = llvm::FunctionType::get(i1, std::vector<llvm::Type *>{}, false);
	  scopes.insertFunc("getb",  llvm::Function::Create(getbType, llvm::Function::ExternalLinkage, "readBoolean", TheModule.get()));

    // char [] gets () TODO:FIX
    llvm::FunctionType *getsType = llvm::FunctionType::get(voidT, std::vector<llvm::Type *>{i32, i8->getPointerTo()}, false);
	  scopes.insertFunc("gets",  llvm::Function::Create(getsType, llvm::Function::ExternalLinkage, "readString", TheModule.get()));


    // int abs (int n)
    llvm::FunctionType *abstype = llvm::FunctionType::get(i32, std::vector<llvm::Type *>{i32}, false);
	  scopes.insertFunc("abs",  llvm::Function::Create(abstype, llvm::Function::ExternalLinkage, "abs", TheModule.get()));

    // int ord(char c)
    llvm::FunctionType *ordtype = llvm::FunctionType::get(i32, std::vector<llvm::Type *>{i8}, false);
	  scopes.insertFunc("ord",  llvm::Function::Create(ordtype, llvm::Function::ExternalLinkage, "ord", TheModule.get()));

    //char chr (int n)
    llvm::FunctionType *chrtype = llvm::FunctionType::get(i8, std::vector<llvm::Type *>{i32}, false);
	  scopes.insertFunc("chr",  llvm::Function::Create(chrtype, llvm::Function::ExternalLinkage, "chr", TheModule.get()));



    //TODO: int strlen (char [] s)
    llvm::FunctionType *strlenType = llvm::FunctionType::get(i32, std::vector<llvm::Type *>{i8->getPointerTo()}, false);
	  scopes.insertFunc("strlen",  llvm::Function::Create(strlenType, llvm::Function::ExternalLinkage, "strlen", TheModule.get()));

    //TODO: int strcmp (char[] s1, s2)
    //TODO: strcpy (char[] trg, src)
    //TODO: strcat (char[] trg, src)
}

llvm::ConstantInt* AST::c1(bool b) {
    if(b) return llvm::ConstantInt::getTrue(TheContext);
    else return llvm::ConstantInt::getFalse(TheContext);
}
llvm::ConstantInt* AST::c8(char c) {
    return llvm::ConstantInt::get(TheContext, llvm::APInt(8, c, true));
}
llvm::ConstantInt* AST::c32(int n) {
    return llvm::ConstantInt::get(TheContext, llvm::APInt(32, n, true));
}
  /*
   * This function creates a pointer to the LLVM type that corresponds
   * to a TonyType. For the case of simple expressions, like integers,
   * LLVM already provides types (e.g. `i32`).
   * BUT for the case of more complex structures, like lists, we
   * must construct the corresponding LLVM type.
   */
llvm::Type* AST::getOrCreateLLVMTypeFromTonyType(TonyType *t, PassMode mode) {
    
    //Initializing to avoid warning
    llvm::Type *retType = i32;
    switch(t->get_current_type()) {
      case TYPE_int:  retType = i32; break;
      case TYPE_bool: retType = i1;  break;
      case TYPE_char: retType = i8;  break;
      case TYPE_any:  retType = i32; break;
      case TYPE_void: retType = voidT;  break;
      case TYPE_list: {
        std::string hash = t->createHashKeyForType();
        llvm::Type* list_type = llvm_list_types.lookup(hash);

        // In this case, we haven't created a type for such a list yet.
        // So, we will create it and insert it in `llvm_list_types`.        
        if (list_type == nullptr) {
          llvm::StructType* node_type = llvm::StructType::create(TheContext, "nodetype");
          llvm::PointerType* pointer_to_node_type = llvm::PointerType::get(node_type, 0);

          // The value of a node can be a list too. So, we must construct its type
          // first by calling `getOrCreateLLVMTypeFromTonyType` recursively.
          llvm::Type* node_value_type = 
            getOrCreateLLVMTypeFromTonyType(t->get_nested_type());
          
          // A list node has type `node_type` and consists of:
          // - a value of type `node_value_type`
          // - a pointer to a list node of type `pointer_to_node_type`
          node_type->setBody({node_value_type, pointer_to_node_type});
          llvm_list_types.insert(hash, pointer_to_node_type);
          list_type = pointer_to_node_type;
        }
        retType = list_type; break;
      }
      case TYPE_array: {
        llvm::Type* element_type =
          getOrCreateLLVMTypeFromTonyType(t->get_nested_type());
        if (t->get_array_size() == 0) {
          llvm::PointerType* array_type =
            llvm::PointerType::get(element_type, 0);
          retType = array_type;
        } else {
        llvm::ArrayType* array_type =
          llvm::ArrayType::get(element_type, t->get_array_size());
        retType = array_type;
        }
        break;
      }
      default: yyerror("Type conversion not implemented yet");
    }

    if(mode == REF) {
      retType = retType->getPointerTo();
    }
    return retType;
  }

  llvm::Type *getLLVMRefType(llvm::Type *orgType){
    return orgType->getPointerTo();
}

llvm::AllocaInst * AST::CreateEntryBlockAlloca (llvm::Function *TheFunction, const std::string &VarName, llvm::Type* Ty){
    llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(Ty, 0, VarName.c_str());
}

/*================================================================================================*/
/*                                            Expr                                                */
/*================================================================================================*/

void Expr::setLLVMType(llvm::Type* t) {
    LLVMType = t;
}

/*================================================================================================*/
/*                                            Id                                                  */
/*================================================================================================*/

llvm::Value * Id::compile(){
    if(!blocks.back()->isRef(var)){
      // By value variable
      llvm::Value *v = blocks.back()->getVal(var);

      return Builder.CreateLoad(v);
    } else{
      // By reference variable
      auto *addr = Builder.CreateLoad(blocks.back()->getAddr(var));
      return Builder.CreateLoad(addr);
    }
  } 

/*================================================================================================*/
/*                                        ArrayElement                                            */
/*================================================================================================*/

llvm::Value *ArrayElement::compile(){
    llvm::Value* array_index = expr->compile();
    llvm::Value *v, *array;
    if(dynamic_cast<Id*>(atom) == nullptr) {
      // In this case, the atom is not an Id (e.g: a[5]), instead
      // it may be another ArrayElement (e.g. a[4][2]) or a function
      // call (e.g. f(arr)[0]). So, we compile the atom to get the
      // array.
      array = atom->compile();
    } else if(blocks.back()->isRef(getName())) {
      // In this case, the atom is an Id (e.g: a[0]), and it is a
      // function parameter that is passed by reference (e.g. ref int[] a).
      v = Builder.CreateLoad(blocks.back()->getAddr(getName()));
      array = Builder.CreateLoad(v, getName().c_str());
    } else {
      // In this case, the atom is an Id (e.g: a[0]), and it is a
      // function parameter that is passed by value.
      v = blocks.back()->getVal(getName());
      array = Builder.CreateLoad(v, getName().c_str());
    }

    llvm::Value* elem_ptr = Builder.CreateGEP(array, array_index);
    if (pass_by_value) {
      // In this case, the ArrayElement is NOT used on the left side
      // of an assignment, but on the right. So, we return the loaded
      // value of the element.
      return Builder.CreateLoad(elem_ptr, "elem");
    }
    return elem_ptr;
} 

/*================================================================================================*/
/*                                        StringLiteral                                           */
/*================================================================================================*/

llvm::Value *StringLiteral::compile() {
    llvm::Value* p =
      Builder.CreateCall(TheMalloc, c32(strlit.length()+1), "strlitaddr");
    p = Builder.CreateBitCast(p, getOrCreateLLVMTypeFromTonyType(type));
    llvm::Value* char_ptr;
    for (std::string::size_type i = 0; i < strlit.length(); i++) {
        char_ptr = Builder.CreateGEP(p, c32(i));
        Builder.CreateStore(c8(strlit[i]), char_ptr);
    }
    char_ptr = Builder.CreateGEP(p, c32(strlit.length()));
    Builder.CreateStore(c8('\0'), char_ptr);
    return p;
} 

/*================================================================================================*/
/*                                        CharConst                                               */
/*================================================================================================*/

llvm::Value *CharConst::compile() {
    return c8(char_const);
} 

/*================================================================================================*/
/*                                        IntConst                                                */
/*================================================================================================*/

llvm::Value *IntConst::compile (){
    return c32(num);
}

/*================================================================================================*/
/*                                          New                                                   */
/*================================================================================================*/

llvm::Value *New::compile(){
    llvm::Value* e = expr->compile();
    llvm::Value* size =
      Builder.CreateMul(e, c32(type_of_elems->get_data_size_of_type()),
                        "sizeformalloc");
    llvm::Value* p =
      Builder.CreateCall(TheMalloc, {size}, "arrayaddr");
    return p;
} 

/*================================================================================================*/
/*                                          Nil                                                   */
/*================================================================================================*/

llvm::Value *Nil::compile(){
    return llvm::ConstantPointerNull::get(llvm::Type::getInt32Ty(TheContext)->getPointerTo());
}

/*================================================================================================*/
/*                                         Boolean                                                */
/*================================================================================================*/

llvm::Value *Boolean::compile(){
    return c1(b);
} 

/*================================================================================================*/
/*                                          BinOp                                                 */
/*================================================================================================*/

llvm::Value *BinOp::compile(){
    
    llvm::Value *l = left->compile();
    llvm::Value *r = right->compile();
    
    if(op ==  "+")          return Builder.CreateAdd(l, r, "addtmp");
    else if(op ==  "-")     return Builder.CreateSub(l, r, "subtmp");
    else if(op ==  "*")     return Builder.CreateMul(l, r, "multmp");
    else if(op ==  "/")     return Builder.CreateSDiv(l, r, "divtmp");
    else if(op ==  "mod")   return Builder.CreateSRem(l, r, "remtmp");
    else if(op ==  "=")     return Builder.CreateICmpEQ(l,r, "eqtmp");
    else if(op ==  "<>")    return Builder.CreateICmpNE(l,r, "neqtmp");
    else if(op ==  "<")     return Builder.CreateICmpSLT(l,r, "slttmp");
    else if(op ==  ">")     return Builder.CreateICmpSGT(l,r, "sgttmp");
    else if(op ==  "<=")    return Builder.CreateICmpSLE(l,r, "sletmp");
    else if(op ==  ">=")    return Builder.CreateICmpSGE(l,r, "sgetmp");
    else if(op ==  "and")   return Builder.CreateAnd(l,r, "andtmp");
    else if(op ==  "or")    return Builder.CreateOr(l,r, "ortmp");
    else if(op ==  "#") {

      LLVMType = getOrCreateLLVMTypeFromTonyType(type);
      llvm::Type* LLVMTypeOfElement =
        getOrCreateLLVMTypeFromTonyType(type->get_nested_type());

      int size = left->get_type()->get_data_size_of_type();
      llvm::Value *p = Builder.CreateCall(TheMalloc, {c32(size)}, "newtmp");
      llvm::Value *n = Builder.CreateBitCast(p, LLVMType, "nodetmp");
      llvm::Value *h = Builder.CreateStructGEP(n, 0, "headptr");
      l = Builder.CreateBitCast(l, LLVMTypeOfElement);
      Builder.CreateStore(l, h);
      llvm::Value *t = Builder.CreateStructGEP(n, 1, "tailptr");
      r = Builder.CreateBitCast(r, LLVMType);
      Builder.CreateStore(r, t);
      return n;
    }
    else                    yyerror("Operation not supported yet");
    return nullptr;
}

/*================================================================================================*/
/*                                           UnOp                                                 */
/*================================================================================================*/

llvm::Value *UnOp::compile(){
    llvm::Value *r = right->compile();

    if (op == "+")     return r;
    if (op == "-")     return Builder.CreateNeg(r);
    if (op == "not")   return Builder.CreateNot(r);
    if (op == "head") {
      // We want to get a pointer to the head, so we must typecast.
			r = Builder.CreateBitCast(
        r, getOrCreateLLVMTypeFromTonyType(right->get_type()));
			
      // We get a pointer to the value of the head.
      r = Builder.CreateStructGEP(r, 0);

      // We load the value.
			r = Builder.CreateLoad(r);
      return r;
    }
    if (op == "tail") {

      // We want to get a pointer to the head, so we must typecast.
			r = Builder.CreateBitCast(
        r, getOrCreateLLVMTypeFromTonyType(right->get_type()));
			
      // We get a pointer to the tail.
      r = Builder.CreateStructGEP(r, 1);

      // We load the value.
			r = Builder.CreateLoad(r);
      return r;
    }
    if (op == "nil?") {
      llvm::Value *nil_type = llvm::ConstantPointerNull::get(static_cast<llvm::PointerType *>(r->getType()));
			return Builder.CreateICmpEQ(r, nil_type);
    }
    return nullptr;
} 

/*================================================================================================*/
/*                                           VarList                                              */
/*================================================================================================*/

llvm::Value *VarList::compile() {
    llvm::Type* t = getOrCreateLLVMTypeFromTonyType(type);
    for (Id * id: ids) {
      llvm::AllocaInst* alloca = Builder.CreateAlloca(t, 0, id->getName());
      blocks.back()->addVar(id->getName(), t, type->getPassMode());
      blocks.back()->addAddr(id->getName(), alloca);
      blocks.back()->addVal(id->getName(), alloca);
    }
    return nullptr;
}

/*================================================================================================*/
/*                                           Formal                                               */
/*================================================================================================*/

llvm::Value *Formal::compile(){
    // Intentionally left empty
    return nullptr;
}

/*================================================================================================*/
/*                                         FormalList                                             */
/*================================================================================================*/

llvm::Value *FormalList::compile(){
    // Intentionally left empty
    return nullptr;
}

/*================================================================================================*/
/*                                           Header                                               */
/*================================================================================================*/

llvm::Value *Header::compile(){
    // Intentionally left empty
    return nullptr;
}

/*================================================================================================*/
/*                                           Return                                               */
/*================================================================================================*/

llvm::Value *Return::compile() {
    llvm::Value* v = ret_expr->compile();

    if (is_nil_constant(ret_expr->get_type())) {
      // If `nil` is returned, we must change its LLVM type
      // (null pointer to an i32) to the return type of the
      // function.
      v = Builder.CreateBitCast(v, blocks.back()->getFun()->getReturnType());
    }
    return Builder.CreateRet(v);
}

/*================================================================================================*/
/*                                           Exit                                                 */
/*================================================================================================*/

llvm::Value* Exit::compile(){
    return Builder.CreateRetVoid();
}

/*================================================================================================*/
/*                                         StmtBody                                               */
/*================================================================================================*/  

llvm::Value *StmtBody::compile(){
    for (Stmt *s : stmts) {
      s->compile();
      if (dynamic_cast<Return*>(s) != nullptr) {
        // In this case, we've reached a `return` statement.
        // So, we don't compile the remaining statements in the
        // statement body.
        has_return = true;
        break;
      }
      if (dynamic_cast<Exit*>(s) != nullptr) {
        // In this case, we've reached a `return` statement.
        // So, we don't compile the remaining statements in the
        // statement body.
        has_exit = true;
        break;
      }
    }
    return nullptr;
}

/*================================================================================================*/
/*                                          Assign                                                */
/*================================================================================================*/ 

llvm::Value* Assign::compile() {
    llvm::Type*  LLVMType = getOrCreateLLVMTypeFromTonyType(atom->get_type());
    llvm::Value* value = expr->compile();
    llvm::Value* variable;

    // As opposed to variables, the address of an array ELEMENT cannot be
    // known beforehand (by looking at the RuntimeTable). So, we run
    // `ArrayElement.compile()` instead of just doing a lookup in the table.
    if (dynamic_cast<ArrayElement*>(atom) != nullptr) {

      // This will inform `ArrayElement.compile()` to return the address of
      // the element, not the actual value inside the element. This is
      // because we want to STORE the value of the right expression into
      // this element.
      atom->setPassByValue(false);
      variable = atom->compile();
      value = Builder.CreateBitCast(value, LLVMType);
      Builder.CreateStore(value, variable);
    } else {
      // This is the case for regular variables.
      if(blocks.back()->isRef(atom->getName())) {
        auto addr = Builder.CreateLoad(blocks.back()->getAddr(atom->getName()));
        value = Builder.CreateBitCast(value, LLVMType);
        Builder.CreateStore(value, addr);
      } else {
        value = Builder.CreateBitCast(value, LLVMType);
        Builder.CreateStore(value, blocks.back()->getVal(atom->getName()));
      }
    }

    return nullptr;
} 

/*================================================================================================*/
/*                                           Skip                                                 */
/*================================================================================================*/ 

llvm::Value *Skip::compile(){
    // Intentionally left empty
    return nullptr;
}

/*================================================================================================*/
/*                                            If                                                  */
/*================================================================================================*/ 

llvm::Value* If::compile() {
    // IMPORTANT: We don't create a new basic block for this statement yet.
    // The condition check for an `if` statement remains in the same
    // basic block as statements before the `if`. BUT, later, we will start
    // a new basic block for the statement body under `then`.
    if (condition == nullptr) {
      // In this case, we are in an `else` statement (no condition).
      if (stmt_body != nullptr) {
        stmt_body->compile();
      }
      return nullptr;
    }

    llvm::Value* cond = condition->compile();
    // QUESTION: Can this ever be true?
    if(!cond) return nullptr;
    // We convert the condition to an LLVM bool
    cond = Builder.CreateICmpNE(cond, c1(0), "ifcond");
    // This is the function in which we're in. We will need this to create
    // basic blocks later on, for `then`, `else` etc... 
    llvm::Function* TheFunction = Builder.GetInsertBlock()->getParent();
    // We create the usual basic blocks (BB)
    llvm::BasicBlock* then_bb  = llvm::BasicBlock::Create(TheContext, "then", TheFunction);
    llvm::BasicBlock* else_bb  = llvm::BasicBlock::Create(TheContext, "else");

    // This is the basic block that all the previous blocks (for `then` and `else`)
    // will jump to, after their execution is finished. This way, the execution
    // will move on past the `if-then-else` statement.
    llvm::BasicBlock* merge_bb = llvm::BasicBlock::Create(TheContext, "exitfromif");

    if(next_if != nullptr) {
      Builder.CreateCondBr(cond, then_bb, else_bb);
    } else {
      Builder.CreateCondBr(cond, then_bb, merge_bb);
    }

    // This is the `then` statement. We start a new basic block.
    Builder.SetInsertPoint(then_bb);
    stmt_body->compile();
    // Basic blocks in LLVM end with either a `ret` or a `br`.
    if(!stmt_body->hasReturnStmt() && !stmt_body->hasExitStmt()) {
      Builder.CreateBr(merge_bb);
    }
    
    if(next_if != nullptr) {
      // Append the basic block for `else` to the list of basic blocks
      // of this function.
      TheFunction->getBasicBlockList().push_back(else_bb);
      // This is the `else` or `elsif` statement. We start a new basic block.
      Builder.SetInsertPoint(else_bb);
      next_if->compile();
      // Next `Ifs` must jump to the current merge block, after they're done.
      if(!next_if->isElse() ||
         (!next_if->getStmtBody()->hasReturnStmt() && !next_if->getStmtBody()->hasExitStmt())) {
        Builder.CreateBr(merge_bb);
      }
    }
    
    // Append the basic block that comes after the whole `if-then-else`
    // statement to the list of basic blocks of this current function.
    TheFunction->getBasicBlockList().push_back(merge_bb);
    // The execution of the program continues in a new basic block.
    Builder.SetInsertPoint(merge_bb);
    return nullptr;
}

/*================================================================================================*/
/*                                        SimpleList                                              */
/*================================================================================================*/ 

llvm::Value *SimpleList::compile(){
    for (auto s: simples){
      s->compile();
    }
    return nullptr;
} 

/*================================================================================================*/
/*                                            For                                                 */
/*================================================================================================*/ 

llvm::Value *For::compile(){
    initializations->compile();

    // Creating new BB for header after current block
    llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();
    llvm::BasicBlock *LoopBB = llvm::BasicBlock::Create(TheContext, "loop", TheFunction);
    llvm::BasicBlock *AfterBB = llvm::BasicBlock::Create(TheContext, "afterloop");

    //Check condition before starting loop
    llvm::Value *cond = condition->compile();
    cond = Builder.CreateICmpNE(cond, c1(0), "loopcond");    
    Builder.CreateCondBr(cond, LoopBB, AfterBB);


    // LoopBB : compiling steps and checking condition  
    Builder.SetInsertPoint(LoopBB);
    stmt_body->compile();
    if(!stmt_body->hasReturnStmt() && !stmt_body->hasExitStmt()) {
      steps->compile();
      cond = condition->compile();
      cond = Builder.CreateICmpNE(cond, c1(0), "loopcond");    
      Builder.CreateCondBr(cond, LoopBB, AfterBB);
    }

    //Emit Merge Block
    TheFunction->getBasicBlockList().push_back(AfterBB);
    Builder.SetInsertPoint(AfterBB);
    
    return nullptr;
} 

/*================================================================================================*/
/*                                          ExprList                                              */
/*================================================================================================*/ 

llvm::Value* ExprList::compile(){
    // Intentionally left empty
    return nullptr;
}

/*================================================================================================*/
/*                                        FunctionCall                                            */
/*================================================================================================*/ 

llvm::Value* FunctionCall::compile(){
    llvm::Function* llvm_function = scopes.getFun(name->getName());
    std::vector<llvm::Value*> compiled_params;
    std::vector<Expr*> parameter_exprs;
    if (hasParams) {
      parameter_exprs = params->get_expr_list();
    }   

    std::vector<TonyType *> functionArgs = name->get_type()->get_function_args();
    std::map<std::string, TonyType*> previousScopeArgs = name->get_type()->getPreviousScopeArgs();

    llvm::Value* v;
    int index = 0;
    auto arg = llvm_function->arg_begin();

    for (auto par: functionArgs) {
      PassMode p = par->getPassMode();
      if(p == VAL) {
        // In this case, the input argument 'arg' is passed BY VALUE in the
        // function.
        v = parameter_exprs[index]->compile();
        if (is_nil_constant(parameter_exprs[index]->get_type())) {
          // If `nil` is passed as an input parameter, we must change
          // its LLVM type (null pointer to an i32) to the type of the
          // corresponding parameter in the function signature.
          v = Builder.CreateBitCast(v, arg->getType());
        }       
      } else {
        // In this case, the input argument 'arg' is passed BY REFERENCE in the
        // function.
        ArrayElement* arr_elem = dynamic_cast<ArrayElement*>(parameter_exprs[index]);
        if (arr_elem != nullptr) {
          // Special case, if 'arg' is an array element, e.g: a[0].
          arr_elem->setPassByValue(false);
          v = arr_elem->compile();
        } else {
          // General case, where 'arg' is a variable
          auto var = dynamic_cast<Id*> (parameter_exprs[index]);
          if(var != nullptr) {
            if(blocks.back()->isRef(var->getName())) {
              v = Builder.CreateLoad(blocks.back()->getAddr(var->getName()));
            } else {
              v = blocks.back()->getVal(var->getName());
            }
          } else {
            v = parameter_exprs[index]->compile();
          }
        }
      }
      compiled_params.push_back(v);
      arg++;
      index++;
    }

    // Iterating through previous Scope args (all references)
    for (auto par: previousScopeArgs) {
      if(blocks.back()->isRef(par.first)) {
        v = Builder.CreateLoad(blocks.back()->getAddr(par.first));
      } else {
        v =  blocks.back()->getVal(par.first);
      }
      compiled_params.push_back(v);
      arg++;
      index++;
    }

    return Builder.CreateCall(llvm_function, compiled_params);
}

/*================================================================================================*/
/*                                     FunctionDeclaration                                        */
/*================================================================================================*/

llvm::Value *FunctionDeclaration::compile(){
    RuntimeBlock* newBlock = new RuntimeBlock();
    blocks.push_back(newBlock);

    std::vector<TonyType*> argTypes = header->getArgs();
    std::vector<std::string> argNames = header->getNames();
    llvm::Type* argLLVMType;
    for(int i=0; i<argTypes.size(); i++) {
      argLLVMType =
        getOrCreateLLVMTypeFromTonyType(argTypes[i], argTypes[i]->getPassMode());
      blocks.back()->addArg(argNames[i], argLLVMType, argTypes[i]->getPassMode());
    }

    //Getting previous scope vars, only gets strings which are not already included in function parameters
    std::map<std::string, TonyType*> previous = functionType->getPreviousScopeArgs();
    for(auto it:previous){
      std::string varname = it.first;
      // First checking if it was already defined as a new function parameter
      if(blocks.back()->containsVar(varname)) continue;

      // Translating type and inserting as a REF parameter
      llvm::Type *translated = getOrCreateLLVMTypeFromTonyType(it.second, REF);
      blocks.back()->addArg(varname, translated, REF);
      argNames.push_back(varname);
      argTypes.push_back(it.second);
    }

    llvm::FunctionType *FT =
      llvm::FunctionType::get(getOrCreateLLVMTypeFromTonyType(header->getType(), header->getType()->getPassMode()),
                              blocks.back()->getArgs(), false);

    llvm::Function *Fun = llvm::Function::Create(FT,llvm::Function::ExternalLinkage, header->getName(), TheModule.get());
    blocks.back()->setFun(Fun);
    scopes.insertFunc(header->getName(), Fun);
    blocks.pop_back();
    return Fun;  
} 

/*================================================================================================*/
/*                                     FunctionDefinition                                         */
/*================================================================================================*/

llvm::Value* FunctionDefinition::compile(){
    RuntimeBlock* newBlock = new RuntimeBlock();
    blocks.push_back(newBlock);

    llvm::Function *Fun = scopes.getFunCurrentScope(header->getName());
    bool isDeclared = (Fun != nullptr);    
    
    std::vector<TonyType*> argTypes = header->getArgs();
    std::vector<std::string> argNames = header->getNames();
    llvm::Type* argLLVMType;
    for(int i=0; i<argTypes.size(); i++) {
      argLLVMType =
        getOrCreateLLVMTypeFromTonyType(argTypes[i], argTypes[i]->getPassMode());
      blocks.back()->addArg(argNames[i], argLLVMType, argTypes[i]->getPassMode());
    }

    //Getting previous scope vars, only gets strings which are not already included in function parameters
    std::map<std::string, TonyType*> previous = functionType->getPreviousScopeArgs();
    for(auto it:previous){
      std::string varname = it.first;
      // First checking if it was already defined as a new function parameter
      if(blocks.back()->containsVar(varname)) continue;

      // Translating type and inserting as a REF parameter
      llvm::Type *translated = getOrCreateLLVMTypeFromTonyType(it.second, REF);
      blocks.back()->addArg(varname, translated, REF);
      argNames.push_back(varname);
      argTypes.push_back(it.second);
    }

    if(!isDeclared){
      llvm::FunctionType *FT =
        llvm::FunctionType::get(getOrCreateLLVMTypeFromTonyType(header->getType(), header->getType()->getPassMode()),
                                blocks.back()->getArgs(), false);

      Fun = llvm::Function::Create(FT,llvm::Function::ExternalLinkage, header->getName(), TheModule.get());
    }

    blocks.back()->setFun(Fun);

    // This will overwrite declared function
    scopes.insertFunc(header->getName(), Fun);
    scopes.openRuntimeScope();
        
    int index = 0;
    for(auto &Arg: Fun->args()){
      Arg.setName(argNames[index++]);
    }
  
    // CREATE Basic Block
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", Fun);
    Builder.SetInsertPoint(BB);    
    blocks.back()->setBlock(BB);

    // Insert Parameters
    for (auto &arg: Fun->args()) {
      llvm::AllocaInst * Alloca = CreateEntryBlockAlloca(Fun, arg.getName().str(), arg.getType());
      if(blocks.back()->isRef(std::string(arg.getName()))) {
        blocks.back()->addAddr(std::string(arg.getName()), Alloca);
      } else {
        blocks.back()->addVal(std::string(arg.getName()), Alloca);
      }
      Builder.CreateStore(&arg, Alloca);
    }

    // Compile other definitions
    for(AST *a: local_definitions) a->compile();
    Builder.SetInsertPoint(BB);

    body->compile();
    
    if (Fun->getReturnType()->isVoidTy()) {
      if (!body->hasExitStmt()) {
			  Builder.CreateRetVoid();
      }
    }
		else { 
			if (!body->hasReturnStmt()) {
        // Return a 'dummy' value for the case that the function
        // has no return statement.
				if (Fun->getReturnType()->isIntegerTy(32))
					Builder.CreateRet(c32(0));
				else if(Fun->getReturnType()->isIntegerTy(8))
					Builder.CreateRet(c8(0));
				else if(Fun->getReturnType()->isIntegerTy(1))
					Builder.CreateRet(c1(0));
        else {
          llvm::Type* ret_type = getOrCreateLLVMTypeFromTonyType(header->getType());
					llvm::ConstantPointerNull* ret_val =
            llvm::ConstantPointerNull::get(llvm::Type::getInt32Ty(TheContext)->getPointerTo());
					Builder.CreateRet(Builder.CreateBitCast(ret_val, ret_type));
        }
			}
		}
    
    blocks.pop_back();
    scopes.closeRuntimeScope();
    if(!isMain)
      Builder.SetInsertPoint(blocks.back()->getCurrentBasicBlock());
    llvm::verifyFunction(*Fun, &llvm::errs());

    return Fun;
} 