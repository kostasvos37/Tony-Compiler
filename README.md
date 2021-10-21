# Tony-Compiler
A compiler for the _Tony_ language... This is part of the _Compilers_ course, at _NTUA-ECE_. To get a basic idea of how a _Tony_ program looks, navigate through the `Tony-Compiler/programs/correct` directory.

The Tony Compiler was developed and tested in a Linux-Ubuntu environment.

### Dependencies:
- _flex_: `2.6.4-6`
- _bison_: `2:3.0.4.dfsg-1build1`
- _libgc-dev_: `1:7.4.2-8ubuntu1`
- _clang-10_
- _llvm-10_
- _python_: `3.7.5` (for running the tests)

### Run a Tony Program
- Change directories to the folder of the project: `cd <your-base-dir>/Tony-Compiler`
- Run: `make tony` to create the `./tony` executable.
- Run: `./tony <tony-program>.tony` to produce the LLVM code of your program.
  A file called `out.ll` is created, and it contains the code.
- Run: `clang-10 out.ll lib.a -o <desired-name>.o -lgc` to produce the binary executable of your program.
- Run the executable: `./<desired-name>.o`.

### Run Tests
- Change directories to the folder of the compiler: `cd <tony-compiler-dir>`
- Run: `make` to create the `./tony` executable.
- Run the tests: `python3 programs/run_tests.py`
