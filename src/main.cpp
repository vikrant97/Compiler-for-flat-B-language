#include <bits/stdc++.h>
#include "Visitor.h"
// #include "global.h"
using namespace std;
extern "C" FILE *yyin;
extern "C" int yyparse();
extern "C" int yylex();
class ASTProgram * start;
extern llvm::Module* module;
int main(int argc, char** argv) {
    if (argc == 1 ) {
    fprintf(stderr, "Correct usage: bcc filename\n");
    exit(1);
    }

    if (argc > 2) {
        fprintf(stderr, "Passing more arguments than necessary.\n");
        fprintf(stderr, "Correct usage: bcc filename\n");
    }

    yyin = fopen(argv[1], "r");

    do {
        yyparse();
    } while (!feof(yyin));
    codegen* printVisitor= new codegen();
    printVisitor->visit(start);
    module->dump();
}