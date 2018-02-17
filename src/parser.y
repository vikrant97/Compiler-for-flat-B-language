%{
#include <bits/stdc++.h>
#include "AST.h"
using namespace std;
extern "C" int yylex();
extern int yylineno;
extern "C" int yyparse();
extern "C" FILE *yyin;
extern class ASTProgram * start;
void yyerror(const char *s);
%}
%token <sval> STRING
%token AND 
%token OR
%token <sval> COLON
%token GOTO
%token PRINTLN
%token <sval>EQUAL
%token <sval>NE
%token <sval>GT
%token <sval>LT
%token <sval>GTE
%token <sval>LTE
%token <sval> MULTE 
%token <sval>PLUSE 
%token <sval> MINUSE 
%token <sval>DIVE
%token <sval>DOUBLEQUOTE
%token FOR
%token WHILE
%token IF
%token ELSE
%token PRINT
%token READ
%token <sval> INT
%token <ival> NUM
%token <sval> ID
%token ETOK
%left <sval>PLUS 
%left<sval>MINUS
%left <sval>MULT 
%left<sval>DIV 
%left<sval> MOD
%token <sval>ASSIGN_OP

%type <program> program
%type <field_declarations> declaration_lists
%type <field_declaration> declaration_list
%type <id_comma> parameters
%type <id_comma> r_parameters
%type <id_comma> p_parameters
%type <statements> statements
%type <statement> statement
%type <location> location
%type <initial> initial
%type <expr> expr
%type <expr> bool_expr
%type <literal> literal
%%

program: 'd' '{' declaration_lists '}' 'c' '{' statements '}' {$$=new ASTProgram($3,$7);start=$$;} 

declaration_lists: declaration_lists declaration_list { $1 -> push_back($2); $$ = $1; } 
				| declaration_list { $$ = new vector < ASTFieldDeclaration * >(); $$ -> push_back($1);};

declaration_list: INT parameters ';' {$$ = new ASTFieldDeclaration($1, $2);} ;

parameters: parameters ',' ID { $1 -> push_back(new ASTVarIdentifier($3)); $$ = $1;}
			| parameters ',' ID '[' NUM ']' { $1 -> push_back(new ASTArrayIdentifier($3, $5)); $$ = $1;}
			| ID  { $$ = new vector < ASTIdComma * >(); $$ -> push_back(new ASTVarIdentifier($1));}
			| ID '[' NUM ']' { $$ = new vector < ASTIdComma * >(); $$ -> push_back(new ASTArrayIdentifier($1, $3));};

statements: statements statement { $1 -> push_back($2); $$ = $1; }; 
				| statement { $$ = new vector<ASTStatement*>(); $$->push_back($1); };
				| statements ID COLON statement { $1->push_back($4);new ASTLabelStatement($2,$4);$$=$1;};

statement: PRINTLN p_parameters ';' { $$ = new ASTNewLineStatement($2);}
		| READ r_parameters ';' {$$ = new ASTReadStatement($2);};
		| PRINT p_parameters ';' {$$ = new ASTPrintStatement($2);};
		| initial ASSIGN_OP expr ';' { $$ = new ASTAssignmentStatement($1, string($2), $3);};
		| initial PLUSE expr ';' { $$ = new ASTAssignmentStatement($1, string($2), $3);};
		| initial MINUSE expr ';' { $$ = new ASTAssignmentStatement($1, string($2), $3);};
		| initial MULTE expr ';' { $$ = new ASTAssignmentStatement($1, string($2), $3);};
		| initial DIVE expr ';' { $$ = new ASTAssignmentStatement($1, string($2), $3);};
		| IF bool_expr '{' statements '}' {$$= new ASTIfStatement($2,$4);};
		| IF bool_expr '{' statements '}' ELSE '{' statements '}' {$$= new ASTIfElseStatement($2,$4,$8);};
		| WHILE bool_expr '{' statements '}' {$$= new ASTWhileStatement($2,$4);};
		| FOR ID ASSIGN_OP NUM ',' NUM '{' statements '}' {$$= new ASTForStatement1($2,$4,$6,$8);};
		| FOR ID ASSIGN_OP NUM ',' NUM ',' NUM '{' statements '}' {$$= new ASTForStatement2($2,$4,$6,$8,$10);};
		| GOTO ID IF bool_expr ';' {$$=new ASTCondGotoStatement($2,$4);};
		| GOTO ID ';' {$$=new ASTUncondGotoStatement($2);};
		
r_parameters: r_parameters ',' ID { $1 -> push_back(new ASTVarIdentifier($3)); $$ = $1;}
			| r_parameters ',' ID '[' NUM ']' { $1 -> push_back(new ASTArrayIdentifier($3, $5)); $$ = $1;}
			| r_parameters ',' ID '[' ID ']' { $1 -> push_back(new ASTArrayIdentifier2($3, $5)); $$ = $1;}
			| ID  { $$ = new vector < ASTIdComma * >(); $$ -> push_back(new ASTVarIdentifier($1));}
			| ID '[' NUM ']' { $$ = new vector < ASTIdComma * >(); $$ -> push_back(new ASTArrayIdentifier($1, $3));}
			| ID '[' ID ']' { $$ = new vector < ASTIdComma * >(); $$ -> push_back(new ASTArrayIdentifier2($1, $3));};

p_parameters: p_parameters ',' ID { $1 -> push_back(new ASTVarIdentifier($3)); $$ = $1;}
			| p_parameters ',' STRING { $1 -> push_back(new ASTPrintString($3)); $$ = $1;}
			| p_parameters ',' NUM { $1 -> push_back(new ASTNumIdentifier($3)); $$ = $1;}
			| p_parameters ',' ID '[' NUM ']' { $1 -> push_back(new ASTArrayIdentifier($3, $5)); $$ = $1;}
			| p_parameters ',' ID '[' ID ']' { $1 -> push_back(new ASTArrayIdentifier2($3, $5)); $$ = $1;}
			| ID  { $$ = new vector < ASTIdComma * >(); $$ -> push_back(new ASTVarIdentifier($1));}
			| STRING { $$ = new vector < ASTIdComma * >(); $$ -> push_back(new ASTPrintString($1));}
			| NUM  { $$ = new vector < ASTIdComma * >(); $$ -> push_back(new ASTNumIdentifier($1));}
			| ID '[' NUM ']' { $$ = new vector < ASTIdComma * >(); $$ -> push_back(new ASTArrayIdentifier($1, $3));}
			| ID '[' ID ']' { $$ = new vector < ASTIdComma * >(); $$ -> push_back(new ASTArrayIdentifier2($1, $3));};

expr:
    location              { $$ = $1; } 
    | literal			{$$=$1;}
    | expr PLUS expr       { $$ = new ASTBinaryExpression($1, string($2), $3); };
    | expr MINUS expr       { $$ = new ASTBinaryExpression($1, string($2), $3);};
    | expr DIV expr       { $$ = new ASTBinaryExpression($1, string($2), $3); };
    | expr MULT expr       { $$ = new ASTBinaryExpression($1, string($2), $3); };
    | expr MOD expr       { $$ = new ASTBinaryExpression($1, string($2), $3); };

initial:
    ID                    { $$ = new ASTVarInitial($1);};
    | ID '[' expr ']'     { $$ = new ASTArrayInitial($1,$3);};

location:
    ID                    { $$ = new ASTVarLocation($1);};
    | ID '[' expr ']'     { $$ = new ASTArrayLocation($1,$3);};

literal:
    NUM                 { $$ = new ASTIntLiteral($1); }
    ;

bool_expr: expr NE expr { $$ = new ASTBinaryExpression($1, string($2), $3); };
				| expr EQUAL expr { $$ = new ASTBinaryExpression($1, string($2), $3); };
				| expr LT expr { $$ = new ASTBinaryExpression($1, string($2), $3); };
				| expr GT expr { $$ = new ASTBinaryExpression($1, string($2), $3); };
				| expr GTE expr { $$ = new ASTBinaryExpression($1, string($2), $3); };
				| expr LTE expr  { $$ = new ASTBinaryExpression($1, string($2), $3); };
%%

void yyerror(const char *s) {
    std::cerr << "Parse Error on Line : " << yylineno << std::endl << "Message : " << s << std::endl;
    exit(0);
}