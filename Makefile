.PHONY: clean distclean default

LLVMCONFIG=llvm-config-10

CXX=clang++-10
CXXFLAGS=-Wall -Wunused-function -Wunneeded-internal-declaration -std=c++11 -g `$(LLVMCONFIG) --cxxflags` 
LDFLAGS=`$(LLVMCONFIG) --ldflags --system-libs --libs all`

default: tonyc

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

lexer.o: lexer.cpp lexer.hpp parser.hpp error.hpp

parser.hpp parser.cpp: parser.y
	bison -dv -o parser.cpp parser.y

parser.o: parser.cpp lexer.hpp error.hpp

tonyc: lexer.o parser.o ast.o semantic.o error.o parsing.o
	$(CXX) $(CXXFLAGS) -o tonyc type.cpp $^ $(LDFLAGS)

clean:
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.o *.hpp.gch


distclean:
	$(RM) tonyc

# This, additionally, cleans the ".asm", ".imm", ".ll", ".o" that are produced
# when running a test program inside the Tony-Compiler directory.
cleanall: distclean
	find . -regex '.*\.\(asm\|imm\|ll\|o\)' -type f -delete

cleanintermidiate:
	find . -regex '.*\.\(asm\|imm\|ll\|o\)' -type f -delete