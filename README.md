# Tony-Compiler
A compiler for Tony language...obviously.

### Run a Tony Program
- Change directories to the folder of the compiler: `cd <tony-compiler-dir>`
- Run: `make` to create the `./tony` executable.
- Run: `./tony <tony-program>.tony` to produce LLVM code.
  A file called `out.ll` is created, and it contains the code.
- Run: `clang out.ll lib.a -o <desired-name>.o` to produce a binary executable.
- Run the executable: `./<desired-name>.o`.

### Run Tests
- Change directories to the folder of the compiler: `cd <tony-compiler-dir>`
- Run: `make` to create the `./tony` executable.
- python3 `programs/run_tests.py`

### The Tony Compiler (surely) Works with:
- _clang_: `10.0.0-4ubuntu1`
- _llvm_: `10.0.0`