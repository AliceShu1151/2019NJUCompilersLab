%option yylineno
%{
#include "syntax.tab.h"
#include "syntax_tree.h"
extern int PRINT_TREE;
int yycolumn = 1;
#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;
      
void Comment_Line();
void Comment_Block();
%}

delim   [ \t]
ws      {delim}+
letter  [A-Za-z]
digit   [0-9]
id      (_|{letter})(_|{letter}|{digit})*
oct     0[0-7]*
hex     0(x|X)({digit}|[A-Fa-f])*
dec     ([1-9]+{digit}*)|0
float   {pointfloat}|{exponentfloat}
pointfloat      ({digitpart})?{fraction}|{digitpart}"."
exponentfloat   {pointfloat}{exponent}
digitpart   {digit}(("_")?{digit})*
fraction    "."{digitpart}
exponent    ("e"|"E")("+"|"-")?{digitpart}
relop       ">"|"<"|">="|"<="|"=="|"!="
type        "int"|"float"
%%

{ws}    {}
"//"    {Comment_Line(); }
"/*"    {Comment_Block(); }
"if"    {yylval = Create_TermNode(yylineno, "IF", IF); return IF;}
"else"  {yylval = Create_TermNode(yylineno, "ELSE", ELSE); return ELSE;}
"while" {yylval = Create_TermNode(yylineno, "WHILE", WHILE); return WHILE;}
"struct"    {yylval = Create_TermNode(yylineno, "STRUCT", STRUCT); return STRUCT;}
"return"    {yylval = Create_TermNode(yylineno, "RETURN", RETURN); return RETURN;}

{type}   {yylval = Create_IDTermNode(yylineno, "TYPE", TYPE, Get_ID(yytext, yyleng)); return TYPE;}
{id}    {yylval = Create_IDTermNode(yylineno, "ID", ID, Get_ID(yytext, yyleng)); return ID;}
{dec}   {yylval = Create_IntTermNode(yylineno, "INT", INT, Get_Dec(yytext)); return INT;}
{oct}   {yylval = Create_IntTermNode(yylineno, "INT", INT, Get_Oct(yytext)); return INT;}
{hex}   {yylval = Create_IntTermNode(yylineno, "INT", INT, Get_Hex(yytext)); return INT;}
{float} {yylval = Create_FloatTermNode(yylineno, "FLOAT", FLOAT,Get_Flt(yytext)); return FLOAT;}

"("     {yylval = Create_TermNode(yylineno, "LP", LP); return LP;}
")"     {yylval = Create_TermNode(yylineno, "RP", RP); return RP;}
"["     {yylval = Create_TermNode(yylineno, "LB", LB); return LB;}
"]"     {yylval = Create_TermNode(yylineno, "RB", RB); return RB;}
"{"     {yylval = Create_TermNode(yylineno, "LC", LC); return LC;}
"}"     {yylval = Create_TermNode(yylineno, "RC", RC); return RC;}
{relop} {yylval = Create_RelopTermNode(yylineno, "RELOP", RELOP, Get_ID(yytext, yyleng)); return RELOP;}
"="     {yylval = Create_TermNode(yylineno, "ASSIGNOP", ASSIGNOP); return ASSIGNOP;}
"+"     {yylval = Create_TermNode(yylineno, "PLUS", PLUS); return PLUS;}
"-"     {yylval = Create_TermNode(yylineno, "MINUS", MINUS); return MINUS;}
"*"     {yylval = Create_TermNode(yylineno, "STAR", STAR); return STAR;}
"/"     {yylval = Create_TermNode(yylineno, "DIV", DIV); return DIV;}
"&&"    {yylval = Create_TermNode(yylineno, "AND", AND); return AND;}
"||"    {yylval = Create_TermNode(yylineno, "OR", OR);  return OR;}
";"     {yylval = Create_TermNode(yylineno, "SEMI", SEMI); return SEMI;}
","     {yylval = Create_TermNode(yylineno, "COMMA", COMMA); return COMMA;}
"."     {yylval = Create_TermNode(yylineno, "DOT", DOT); return DOT;}
"!"     {yylval = Create_TermNode(yylineno, "NOT", NOT); return NOT;}

\n      {yycolumn=1;}
.       {PRINT_TREE = 0; printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno,yytext);}
%%

void Comment_Line() {
    char c=input(); 
    while (c!='\n' && c!=EOF) 
        c=input();
}

void Comment_Block() {
    char c=input(); 
    while (c!=EOF) {
        if (c=='*') {
            c=input(); 
            if (c=='/')  
                break;
        }
        else 
            c=input();
    }
}

