.PHONY: clean distclean default

LLVMCONFIG=llvm-config-10

CXX=clang++-10
CXXFLAGS=-Wall -Wunused-function -Wunneeded-internal-declaration -std=c++11 -g `$(LLVMCONFIG) --cxxflags` 
LDFLAGS=`$(LLVMCONFIG) --ldflags --system-libs --libs all`

default: tonyc

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

lexer.o: lexer.cpp lexer.hpp parser.hpp error/error.hpp

parser.hpp parser.cpp: parser.y
	bison -dv -o parser.cpp parser.y

parser.o: parser.cpp lexer.hpp error/error.hpp

tonyc: lexer.o parser.o llvm/ast.o symbol/type.o semantic/semantic.o error/error.o parser/parsing.o
	$(CXX) $(CXXFLAGS) -o tonyc $^ $(LDFLAGS)

clean:
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.o *.hpp.gch semantic/semantic.o parser/parsing.o error/error.o llvm/ast.o

distclean:
	$(RM) tonyc

# This, additionally, cleans the ".asm", ".imm", ".ll", ".o" that are produced
# when running a test program inside the Tony-Compiler directory.
cleanall: distclean
	find . -regex '.*\.\(asm\|imm\|ll\|o\|out\)' -type f -delete

cleanintermidiate:
	find . -regex '.*\.\(asm\|imm\|ll\|o\|out\)' -type f -delete