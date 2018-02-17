%{
#include<bits/stdc++.h>
#include "AST.h"
#include "parser.tab.h"
#define YY_DECL extern "C" int yylex()
%}

%%
"declblock"			{return 'd';}
"codeblock"			{return 'c';}
"int"				{ yylval.sval=strdup(yytext);return INT; }
"print"    			{return PRINT;}
"read"				{return READ;}
"for" 				{return FOR;}
"while" 			{return WHILE;}
"if" 				{return IF;}
"else" 				{return ELSE;}
"goto" 				{return GOTO;}
"println" 			{return PRINTLN;}
"and"				{return AND;}
"or"				{return OR;}
[0-9][0-9]*			{yylval.ival=atoi(yytext);return NUM;} 
[a-zA-Z][a-zA-Z0-9]* {yylval.sval=strdup(yytext);return ID;}

"="     {yylval.sval=strdup(yytext);return ASSIGN_OP;} 
"=="	{yylval.sval=strdup(yytext);return EQUAL;}
">"		{yylval.sval=strdup(yytext);return GT;}
"<"		{yylval.sval=strdup(yytext);return LT;}
">="	{yylval.sval=strdup(yytext);return GTE;}
"<="	{yylval.sval=strdup(yytext);return LTE;}
"!="	{yylval.sval=strdup(yytext);return NE;}
"+"		{yylval.sval=strdup(yytext);return PLUS;}
"-"		{yylval.sval=strdup(yytext);return MINUS;}	
"%"		{yylval.sval=strdup(yytext);return MOD;}
"/"     {yylval.sval=strdup(yytext);return DIV;}
"*"		{yylval.sval=strdup(yytext);return MULT;}
"*="	{yylval.sval=strdup(yytext);return MULTE;}
"/="	{yylval.sval=strdup(yytext);return DIVE;}
"+="	{yylval.sval=strdup(yytext);return PLUSE;}
"-="	{yylval.sval=strdup(yytext);return MINUSE;}
"{"		{return '{';}
"}"		{return '}';}
"("		{return '(';}
")"		{return ')';}
"["		{return '[';}
"]"		{return ']';}
";"		{return ';';}
","		{return ',';}
":"		{yylval.sval=strdup(yytext);return COLON;}
"\""		{return DOUBLEQUOTE;}
"\""([^\n\"\\]*(\\[.\n])*)*"\"" {yylval.sval=strdup(yytext);return STRING;}
[ \t\n]		{;}
.		{ printf("Unexpected token encountered: %s\n", yytext); return ETOK;}
