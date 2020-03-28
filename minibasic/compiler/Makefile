.PHONY: clean distclean default

CXX=c++
CXXFLAGS=-Wall -std=c++11

default: minibasic

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

lexer.o: lexer.cpp lexer.hpp parser.hpp ast.hpp

parser.hpp parser.cpp: parser.y
	bison -dv -o parser.cpp parser.y

parser.o: parser.cpp lexer.hpp ast.hpp

minibasic: lexer.o parser.o
	$(CXX) $(CXXFLAGS) -o minibasic lexer.o parser.o

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $^

libminibasic.a: writeString.o writeInteger.o formatInteger.o
	$(AR) $(ARFLAGS) $@ $^

clean:
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.o

distclean: clean
	$(RM) minibasic