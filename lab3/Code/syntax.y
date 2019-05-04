%{
#include "syntax_tree.h"
//#include "semantic_analysis.h"
#include "intercode_translate.h"
#define YYSTYPE TreeNode*
#define YYERROR_VERBOSE 1
#include "lex.yy.c"

int PRINT_TREE = 1;

void yyerror(const char* msg) {
    PRINT_TREE = 0;
    printf("Error type B at Line %d: %s\n", yylineno, msg);
}

%}

%token TYPE ID INT FLOAT  
%token LC RC IF WHILE STRUCT  RETURN
%token SEMI COMMA

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT

%%

/* High-level Definitions */
Program :   ExtDefList  {   
                //printf("%d\n", @$.first_line);
                $$ = Create_NonTermNode(@$.first_line, "Program");
                Add_Children($$, 1, $1);
                if (PRINT_TREE) {
                    //Print_Tree($$);
                    semantic_analysis($$);
                    if (!has_semantic_error()) {
                        intercode_translate($$);
                    }
                }
            }
        ;   
ExtDefList  :   ExtDef ExtDefList {   
                    $$ = Create_NonTermNode(@$.first_line, "ExtDefList");
                    Add_Children($$, 2, $1, $2);
                }
            |   {   
                    $$ = NULL;
                }   
            ;
ExtDef   :   Specifier ExtDecList SEMI {   
                $$ = Create_NonTermNode(@$.first_line, "ExtDef");
                Add_Children($$, 3, $1, $2, $3);
            }
        |   Specifier SEMI {   
                $$ = Create_NonTermNode(@$.first_line, "ExtDef");
                Add_Children($$, 2, $1, $2);
            }
        |   Specifier FunDec CompSt {   
                $$ = Create_NonTermNode(@$.first_line, "ExtDef");
                Add_Children($$, 3, $1, $2, $3);
            }
        |   Specifier FunDec SEMI {
                $$ = Create_NonTermNode(@$.first_line, "ExtDef");
                Add_Children($$, 3, $1, $2, $3);
            }
        |   Specifier error SEMI
        |   error ExtDecList SEMI
        |   error SEMI
        ;
ExtDecList  :   VarDec {   
                    $$ = Create_NonTermNode(@$.first_line, "ExtDecList");
                    Add_Children($$, 1, $1);
                }
            |   VarDec COMMA ExtDecList {   
                    $$ = Create_NonTermNode(@$.first_line, "ExtDecList");
                    Add_Children($$, 3, $1, $2, $3);
            }
            ;
/* Specifier */
Specifier   :   TYPE {   
                    $$ = Create_NonTermNode(@$.first_line, "Specifier");
                    Add_Children($$, 1, $1);
                }
            |   StructSpecifier {   
                    $$ = Create_NonTermNode(@$.first_line, "Specifier");
                    Add_Children($$, 1, $1);
                }
            ;
StructSpecifier :   STRUCT OptTag LC DefList RC  {   
                        $$ = Create_NonTermNode(@$.first_line, "StructSpecifier");
                        Add_Children($$, 5, $1, $2, $3, $4, $5);
                    }         
                |   STRUCT Tag {   
                        $$ = Create_NonTermNode(@$.first_line, "StructSpecifier");
                        Add_Children($$, 2, $1, $2);
                    }
                ;
OptTag  :   ID {   
                $$ = Create_NonTermNode(@$.first_line, "OptTag");
                Add_Children($$, 1, $1);
            }
        |   {
                $$ = NULL;
            }
        ;
Tag :   ID {   
            $$ = Create_NonTermNode(@$.first_line, "Tag");
            Add_Children($$, 1, $1);
        } 
    ;
/* Declarators */
VarDec  :   ID {   
                $$ = Create_NonTermNode(@$.first_line, "VarDec");
                Add_Children($$, 1, $1);
            } 
        |   VarDec LB INT RB {   
                $$ = Create_NonTermNode(@$.first_line, "VarDec");
                Add_Children($$, 4, $1, $2, $3, $4);
            }
        |   VarDec LB error RB 
        ;
FunDec  :   ID LP VarList RP {   
                $$ = Create_NonTermNode(@$.first_line, "FunDec");
                Add_Children($$, 4, $1, $2, $3, $4);
            } 
        |   ID LP RP {   
                $$ = Create_NonTermNode(@$.first_line, "FunDec");
                Add_Children($$, 3, $1, $2, $3);
            } 
        |   ID LP error RP
        ;
VarList :   ParamDec COMMA VarList {   
                $$ = Create_NonTermNode(@$.first_line, "VarList");
                Add_Children($$, 3, $1, $2, $3);
            } 
        |   ParamDec {   
                $$ = Create_NonTermNode(@$.first_line, "VarList");
                Add_Children($$, 1, $1);
            }       
        ;
ParamDec:   Specifier VarDec {   
                $$ = Create_NonTermNode(@$.first_line, "ParamDec");
                Add_Children($$, 2, $1, $2);
            }
        ;
/* Statements */
CompSt  :   LC DefList StmtList RC {   
                $$ = Create_NonTermNode(@$.first_line, "CompSt");
                Add_Children($$, 4, $1, $2, $3, $4);
            }
        |   LC error RC
        ;
StmtList :   Stmt StmtList {   
                $$ = Create_NonTermNode(@$.first_line, "StmtList");
                Add_Children($$, 2, $1, $2);
            }
        |   {
                $$ = NULL;
            }
        ;
Stmt:   Exp SEMI {   
            $$ = Create_NonTermNode(@$.first_line, "Stmt");
            Add_Children($$, 2, $1, $2);
        }
    |   CompSt {   
            $$ = Create_NonTermNode(@$.first_line, "Stmt");
            Add_Children($$, 1, $1);
        }
    |   RETURN Exp SEMI {   
            $$ = Create_NonTermNode(@$.first_line, "Stmt");
            Add_Children($$, 3, $1, $2, $3);
        }
    |   IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {   
            $$ = Create_NonTermNode(@$.first_line, "Stmt");
            Add_Children($$, 5, $1, $2, $3, $4, $5);
        }
    |   IF LP Exp RP Stmt ELSE Stmt {   
            $$ = Create_NonTermNode(@$.first_line, "Stmt");
            Add_Children($$, 7, $1, $2, $3, $4, $5, $6, $7);
        }
    |   WHILE LP Exp RP Stmt {   
            $$ = Create_NonTermNode(@$.first_line, "Stmt");
            Add_Children($$, 5, $1, $2, $3, $4, $5);
        }
    ;
/* Local Definition */                       
DefList :   Def DefList {   
                $$ = Create_NonTermNode(@$.first_line, "DefList");
                Add_Children($$, 2, $1, $2);
            }
        |   {
                $$ = NULL;
            }
        ;
Def :   Specifier DecList SEMI {   
            $$ = Create_NonTermNode(@$.first_line, "Def");
            Add_Children($$, 3, $1, $2, $3);
        }
    |   error SEMI
    ;
DecList :   Dec {   
                $$ = Create_NonTermNode(@$.first_line, "DecList");
                Add_Children($$, 1, $1);
            }
        |   Dec COMMA DecList {   
                $$ = Create_NonTermNode(@$.first_line, "DecList");
                Add_Children($$, 3, $1, $2, $3);
            }
        ;
Dec :   VarDec {   
                $$ = Create_NonTermNode(@$.first_line, "Dec");
                Add_Children($$, 1, $1);
            }
    |   VarDec ASSIGNOP Exp {   
            $$ = Create_NonTermNode(@$.first_line, "Dec");
            Add_Children($$, 3, $1, $2, $3);
        }                                
    ;
/* Expressions */ 
Exp :   Exp ASSIGNOP Exp {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 3, $1, $2, $3);
        }   
    |   Exp AND Exp {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 3, $1, $2, $3);
        }   
    |   Exp OR Exp {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 3, $1, $2, $3);
        }   
    |   Exp RELOP Exp {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 3, $1, $2, $3);
        }   
    |   Exp PLUS Exp {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 3, $1, $2, $3);
        }   
    |   Exp MINUS Exp {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 3, $1, $2, $3);
        }   
    |   Exp STAR Exp {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 3, $1, $2, $3);
        }   
    |   Exp DIV Exp {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 3, $1, $2, $3);
        }   
    |   LP Exp RP {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 3, $1, $2, $3);
        }   
    |   MINUS Exp {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 2, $1, $2);
        }   
    |   NOT Exp {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 2, $1, $2);
        }  
    |   ID LP Args RP {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 4, $1, $2, $3, $4);
        }  
    |   ID LP RP {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 3, $1, $2, $3);
        }   
    |   Exp LB Exp RB {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 4, $1, $2, $3, $4);
        }  
    |   Exp DOT ID {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 3, $1, $2, $3);
        }   
    |   ID {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 1, $1);
        }   
    |   INT {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 1, $1);
        }
    |   FLOAT {   
            $$ = Create_NonTermNode(@$.first_line, "Exp");
            Add_Children($$, 1, $1);
        }
    |   LP error RP
    |   ID LP error RP
    |   Exp LB error RB
    ;
Args:   Exp COMMA Args {   
            $$ = Create_NonTermNode(@$.first_line, "Args");
            Add_Children($$, 3, $1, $2, $3);
        }  
    |   Exp {   
            $$ = Create_NonTermNode(@$.first_line, "Args");
            Add_Children($$, 1, $1);
        }
    ;
%%

    







                        

