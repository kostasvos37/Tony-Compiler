.PHONY: clean distclean default

CXX=c++
CXXFLAGS=-Wall

default: tony

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

lexer.o: lexer.cpp lexer.hpp parser.hpp

parser.hpp parser.cpp: parser.y
	bison -dv -o parser.cpp parser.y

parser.o: parser.cpp lexer.hpp

minibasic: lexer.o parser.o
	$(CXX) $(CXXFLAGS) -o minibasic lexer.o parser.o

lextest: lexer.cpp
	$(CXX) $(CXXFLAGS) -o tony lexer.cpp
	

clean:
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.o

distclean: clean
	$(RM) minibasic
