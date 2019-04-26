#include "syntax_tree.h"
#include "syntax.tab.h"


TreeNode *Create_TreeNode(int lineno, int is_term, const char *tokenname, int tokentype) 
{
    TreeNode *treenode = malloc(sizeof(TreeNode));
    treenode->lineno = lineno;
    treenode->type = is_term;
    treenode->tokenname = tokenname;
    treenode->tokentype = tokentype;
    treenode->child = NULL;
    treenode->brother = NULL;
    return treenode;
}

TreeNode *Create_TermNode(int lineno, const char *tokenname, int tokentype) 
{
    TreeNode *treenode = Create_TreeNode(lineno, TERMINAL, tokenname, tokentype);
    return treenode;
}

TreeNode *Create_NonTermNode(int lineno, const char *tokenname) 
{
    TreeNode *treenode = Create_TreeNode(lineno, NON_TERM, tokenname, NO_TERM_TOKEN_TYPE);
    return treenode;
}


char *Get_ID(const char *text, int leng) 
{
    char *id;
    id = malloc(leng*sizeof(char)+1);
    strcpy(id, text);
    //printf("%s\n", id);
    //printf("%s\n", text);
    return id;
}

float Get_Flt(const char *text) 
{
    float fval;
    sscanf(text, "%f", &fval);
    return fval;
}

int Get_Hex(const char *text) 
{
    int ival;
    sscanf(text, "%x", &ival);
    return ival;
}

int Get_Dec(const char *text) 
{
    int ival;
    sscanf(text, "%d", &ival);
    return ival;
}

int Get_Oct(const char *text) 
{
    int ival;
    sscanf(text, "%o", &ival);
    return ival;
}

TreeNode *Create_IDTermNode(int lineno, const char *tokenname, int tokentype, const char *text) 
{
    TreeNode *treenode = Create_TreeNode(lineno, NON_TERM, tokenname, tokentype);
    treenode->idname = text;
    return treenode;
}

TreeNode *Create_FloatTermNode(int lineno, const char *tokenname, int tokentype, float fval) 
{
    TreeNode *treenode = Create_TreeNode(lineno, NON_TERM, tokenname, tokentype);
    treenode->fval = fval;
    return treenode;
}

TreeNode *Create_IntTermNode(int lineno, const char *tokenname, int tokentype, int ival) 
{
    TreeNode *treenode = Create_TreeNode(lineno, NON_TERM, tokenname, tokentype);
    treenode->ival = ival;
    return treenode;
}


void Add_Children(TreeNode *parent_node, int bro_num,...) 
{
    va_list valist;
    int i;
    TreeNode *last, *next;
    
    va_start(valist, bro_num);
    for (i = 0; i < bro_num; i++) 
    {
        next = va_arg(valist, TreeNode *);
        if (next != NULL) 
        {
            if (parent_node->child == NULL)
                parent_node->child = next;
            else
                last->brother = next;  
            last = next;
        }
    }
    va_end(valist);
}


void print_children(TreeNode *root, int depth) 
{
    int i;
    TreeNode *next;
    if (root == NULL)
        return;
        
    if (root->tokentype == ID)
        printf("ID: %s\n", root->idname);
    else if (root->tokentype == TYPE)
        printf("TYPE: %s\n", root->idname);
    else if (root->tokentype == INT)
        printf("INT: %d\n", root->ival);
    else if (root->tokentype == FLOAT)
        printf("FLOAT: %f\n", root->fval);
    else if (root->type == TERMINAL)
        printf("%s\n", root->tokenname);
    else if (root->type == NON_TERM)
        printf("%s (%d)\n", root->tokenname, root->lineno);
    depth = depth + 1;
    next = root->child;
    while (next != NULL) 
    {
        for (i = 0; i < depth; i++)
            printf("  ");
        print_children(next, depth);
        next = next->brother;
    }
    
}

void Print_Tree(TreeNode *root) 
{
    print_children(root, 0);
}






