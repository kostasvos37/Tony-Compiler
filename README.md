# Tony-Compiler
A compiler for the _Tony_ language... This is part of the _Compilers_ course, at _NTUA-ECE_. To get a basic idea of how a _Tony_ program looks, navigate through the `Tony-Compiler/programs/correct` directory.

The Tony Compiler was developed and tested in a Linux-Ubuntu environment.

### Dependencies:
- _flex_=`2.6.4-6`
- _bison_=`2:3.0.4.dfsg-1build1`
- _libgc-dev_=`1:7.4.2-8ubuntu1`
- _clang-10_
- _llvm-10_
- _python_>=`3.7.5` (for running tests and the `tony` bash script)

### Run a Tony Program (`tonyc` executable)
- Change directories to the folder of the project: `cd <your-base-dir>/Tony-Compiler`
- Run: `make tonyc` to create the `./tonyc` executable.
- Run: `./tonyc <tony-program>.tony` to produce the LLVM code of your program.
  A file called `out.ll` is created, and it contains the LLVM code.
- Run: `clang-10 out.ll lib.a -o <desired-name>.o -lgc` to produce the binary executable of your program.
- Run your program executable: `./<desired-name>.o`.

### Alternative Way to Run a Tony Program (`tony` bash script)
- Change directories to the folder of the project: `cd <your-base-dir>/Tony-Compiler`
- Run: `make tonyc` to create the `./tonyc` executable.
- Run: `chmod +x tony` so you can run the `tony` script.
- Run: `./tony <tony-program>.tony`. A `<tony-program>.o` file is produced in the directory
of your Tony program. In the same directory, you'll also find `<tony-program>.asm` and
`<tony-program>.imm` files with the produced assembly code and
intermediate code of your program.
Also the script accepts the flags `-O`, `-f` and `-i`:
  - `-O` is for optimizations.
  - `-f` is for reading the Tony program from `stdin` and printing the final machine code in `stdout`.
  - `-i` is for reading the Tony program from `stdin` and printing the LLVM IR to `stdout`.

  So, in this case you can run: `./tony -O -f -i <tony-program>.tony`.
- Run your program executable: `./<tony-program>.o`.


### Run Tests
- Change directories to the folder of the compiler: `cd <tony-compiler-dir>`
- Run: `make tonyc` to create the `./tonyc` executable.
- Run the tests: `python3 programs/run_tests.py`
