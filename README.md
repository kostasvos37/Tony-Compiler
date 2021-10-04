# Tony-Compiler
A compiler for Tony language...obviously.

### Run a Tony Program
- cd Tony-Compiler
- make
- ./tony my_program.tony (this produces LLVM code
  in a file called: `out.ll`)
- clang out.ll lib.a -o name-of-executable.o
