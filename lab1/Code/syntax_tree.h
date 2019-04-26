#ifndef _SYNTAX_TREE_H
#define _SYNTAX_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define TERMINAL    1
#define NON_TERM    0
#define NO_TERM_TOKEN_TYPE  0

typedef struct TreeNode {
    int lineno;
    int type;           //1:terminal 0:not terminal
    const char *tokenname;      
    int tokentype;      //defined in syntax.tab.h
    union {
        const char *idname;
        int ival;
        float fval;
    };
    struct TreeNode *child;
    struct TreeNode *brother;
} TreeNode;

TreeNode *Create_TreeNode(int lineno, int is_term, const char *tokenname, int tokentype);
TreeNode *Create_TermNode(int lineno, const char *tokenname, int tokentype);
TreeNode *Create_NonTermNode(int lineno, const char *tokenname);

char *Get_ID(const char *text, int leng);
float Get_Flt(const char *text);
int Get_Hex(const char *text);
int Get_Dec(const char *text);
int Get_Oct(const char *text);

TreeNode *Create_IDTermNode(int lineno, const char *tokenname, int tokentype, const char *text);
TreeNode *Create_FloatTermNode(int lineno, const char *tokenname, int tokentype, float fval);
TreeNode *Create_IntTermNode(int lineno, const char *tokenname, int tokentype, int ival);

void Add_Children(TreeNode *parent_node, int bro_num,...);

void Print_Tree(TreeNode *root);

#endif
