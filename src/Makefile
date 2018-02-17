bcc:  parser.tab.c lex.yy.c main.cpp AST.h Visitor.h codegen.cpp
	g++ parser.tab.c lex.yy.c main.cpp codegen.cpp -std=c++11 -fpermissive -w -Wno-deprecated -g -lfl  `llvm-config-3.8 --cxxflags --cppflags mcjit native --ldflags --libs core` -ltinfo -lpthread -ldl -o bcc
parser.tab.c: parser.y 
	bison -d parser.y 
lex.yy.c: scanner.l 
	flex scanner.l
.PHONY: clean 
clean:
	-@rm -rf parser.tab.c parser.tab.h lex.yy.c bcc 2>/dev/null || true
