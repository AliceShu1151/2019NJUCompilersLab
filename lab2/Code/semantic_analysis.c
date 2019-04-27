#include "semantic_analysis.h"

void semantic_analysis_r(TreeNode *node);

void semantic_extdef(TreeNode *node);

void semantic_extdeclist(TreeNode *node, type_t *type);
type_t *semantic_specifier(TreeNode *node);
type_t *semantic_structspecifier(TreeNode *node);
void semantic_deflist(TreeNode *node, int where, field_list_t *field_list);
void semantic_declist(TreeNode *node, type_t *type, field_list_t *field_list, int where);
void semantic_dec(TreeNode *node, type_t *type, field_list_t *field_list, int where);
void semantic_vardec(TreeNode *node, symbol_t *symbol, type_t *type);

void semantic_fundec(TreeNode *node, type_t *type, symbol_t *symbol);
void semantic_varlist(TreeNode *node, field_list_t *field_list);
void semantic_paramdec(TreeNode *node, field_list_t *field_list);

void symbol_table_check_add(symbol_t *symbol);

void struct_table_check_add(type_struct_t *new_struct, int lineno);

void field_list_check_push_back(field_list_t *field_list, symbol_t *symbol);

void param_list_check_add(symbol_t *symbol, field_list_t *param_list);

void semantic_analysis(TreeNode *root)
{
    Print_Tree(root);
    init_struct_type_table();
    init_symbol_table();
    semantic_analysis_r(root);
    //print_struct_type_table();
    print_symbol_table();
    printf("fuck!\n");
}

void semantic_analysis_r(TreeNode *node)
{
    if (node == NULL)
    {
        return;
    }

    if (strcmp(node->tokenname, "ExtDef") == 0)
    {
        semantic_extdef(node);
        return;
    }

    for (TreeNode *child = node->child; child != NULL; child = child->brother)
    {
        semantic_analysis_r(child);
    }
}

void semantic_extdef(TreeNode *node)
{
    assert(strcmp(node->tokenname, "ExtDef") == 0);
    TreeNode *specifier = node->child;
    type_t *type_specifier = semantic_specifier(specifier);
    if (type_specifier == NULL)
    {
        return;
    }

    TreeNode *specifier_next = specifier->brother;
    if (strcmp(specifier_next->tokenname, "ExtDecList") == 0)
    {
        semantic_extdeclist(specifier_next, type_specifier);
    }
    else if (strcmp(specifier_next->tokenname, "SEMI") == 0)
    {
        return;
    }
    else if (strcmp(specifier_next->tokenname, "FunDec") == 0)
    {
        symbol_t *symbol = create_symbol();
        semantic_fundec(specifier_next, type_specifier, symbol);
        printf("2\n");
    }
}

type_t *semantic_specifier(TreeNode *node)
{
    assert(strcmp(node->tokenname, "Specifier") == 0);

    TreeNode *specifier_child = node->child;
    if (strcmp(specifier_child->tokenname, "TYPE") == 0)
    {
        if (strcmp(specifier_child->idname, "int") == 0)
        {
            return (type_t *)create_type_basic(TYPE_BASIC_INT);
        }
        else if (strcmp(specifier_child->idname, "float") == 0)
        {
            return (type_t *)create_type_basic(TYPE_BASIC_FLOAT);
        }
    }
    else if (strcmp(specifier_child->tokenname, "StructSpecifier") == 0)
    {
        return (type_t *)semantic_structspecifier(specifier_child);
    }
    return NULL;
}

type_t *semantic_structspecifier(TreeNode *node)
{
    assert(strcmp(node->tokenname, "StructSpecifier") == 0);
    assert(strcmp(node->child->tokenname, "STRUCT") == 0);
    //printf("Struct\n");
    TreeNode *struct_next = node->child->brother;
    type_t *specifier_struct;

    if (strcmp(struct_next->tokenname, "OptTag") == 0)
    {
        TreeNode *struct_id = struct_next->child;
        assert(strcmp(struct_id->tokenname, "ID") == 0);
        field_list_t *field_list = create_field_list();
        if (strcmp(struct_next->brother->brother->tokenname, "RC") == 0)
        {
            field_list = NULL;
        }
        else
        {
            semantic_deflist(struct_next->brother->brother, STRUCT_SPECIFIER, field_list);
        }
        specifier_struct = (type_t *)create_type_struct(struct_id->idname, field_list);
        struct_table_check_add((type_struct_t *)specifier_struct, struct_next->lineno);
        return specifier_struct;
    }
    else if (strcmp(struct_next->tokenname, "Tag") == 0)
    {
        specifier_struct = struct_table_find_name(struct_next->child->idname);
        if (specifier_struct == NULL)
        {
            print_semantic_error(17, node->lineno, struct_next->child->idname);
        }
        return (type_t *)specifier_struct;
    }
}
void semantic_deflist(TreeNode *node, int where, field_list_t *field_list)
{
    assert(strcmp(node->tokenname, "DefList") == 0);

    TreeNode *deflist = node;
    while (deflist != NULL)
    {
        TreeNode *def = deflist->child;
        assert(strcmp(def->tokenname, "Def") == 0);
        type_t *type_specifier = semantic_specifier(def->child);
        if (type_specifier != NULL)
        {
            semantic_declist(def->child->brother, type_specifier, field_list, where);
        }
        deflist = def->brother;
    }
}

void semantic_declist(TreeNode *node, type_t *type, field_list_t *field_list, int where)
{
    assert(strcmp(node->tokenname, "DecList") == 0);

    //printf("DecList\n");
    TreeNode *declist = node;
    while (declist != NULL)
    {
        TreeNode *dec = declist->child;
        semantic_dec(dec, type, field_list, where);
        if (dec->brother != NULL)
        {
            assert(strcmp(dec->brother->tokenname, "COMMA") == 0);
            declist = dec->brother->brother;
        }
        else
        {
            declist = dec->brother;
        }
    }
}

void semantic_dec(TreeNode *node, type_t *type, field_list_t *field_list, int where)
{
    assert(strcmp(node->tokenname, "Dec") == 0);

    symbol_t *symbol = create_symbol();
    semantic_vardec(node->child, symbol, type);
    if (where == STRUCT_SPECIFIER)
    {
        field_list_check_push_back(field_list, symbol);
    }
    else if (where == STATEMENT)
    {
        symbol_table_check_add(symbol);
    }
    TreeNode *assignop = node->brother;
    if (assignop == NULL)
    {
        return;
    }
    if (strcmp(assignop->tokenname, "ASSIGNOP") == 0)
    {
        print_semantic_error(15, node->lineno, node->tokenname);
    }
}

void semantic_vardec(TreeNode *node, symbol_t *symbol, type_t *type)
{
    assert(strcmp(node->tokenname, "VarDec") == 0);
    if (strcmp(node->child->tokenname, "ID") == 0)
    {
        init_symbol(symbol, node->child->idname, type, node->child->lineno, DEFINED);
    }
    else if (strcmp(node->child->tokenname, "VarDec") == 0)
    {
        if (strcmp(node->child->brother->brother->tokenname, "FLOAT") == 0)
        {
            print_semantic_error(4, node->child->brother->brother->lineno,
                                 node->child->brother->brother->fval);
        }
        else if (strcmp(node->child->brother->brother->tokenname, "INT") == 0)
        {
            int size = node->child->brother->brother->ival;
            type_t *array_type = (type_t *)create_type_array(size, type);
            semantic_vardec(node->child, symbol, array_type);
        }
    }
}

void semantic_extdeclist(TreeNode *node, type_t *type)
{
    assert(strcmp(node->tokenname, "ExtDecList") == 0);

    symbol_t *symbol = create_symbol();
    semantic_vardec(node->child, symbol, type);
    symbol_table_check_add(symbol);
    print_symbol(symbol);
    if (node->child->brother != NULL)
    {
        semantic_extdeclist(node->child->brother->brother, type);
    }
}

void semantic_fundec(TreeNode *node, type_t *type, symbol_t *symbol)
{
    assert(strcmp(node->tokenname, "FunDec") == 0);
    TreeNode *fundec_id = node->child;
    field_list_t *field_list = create_field_list();
    type_list_t *param_list;
    init_type_list(param_list);
    if (strcmp(fundec_id->brother->brother->tokenname, "RP") == 0)
    {
        param_list = NULL;
    }
    else
    {
        semantic_varlist(fundec_id->brother->brother, field_list);
    }
    field_list_add_to_type_list(field_list, param_list);
    type_t *type_func = (type_t *)create_type_func(type, param_list);
    init_symbol(symbol, fundec_id->idname, type_func, fundec_id->lineno, NOT_DEFINED);
    symbol_table_check_add(symbol);
    printf("1\n");
}

void semantic_varlist(TreeNode *node, field_list_t *field_list)
{
    assert(strcmp(node->tokenname, "VarList") == 0);
    semantic_paramdec(node->child, field_list);

    if (node->child->brother != NULL)
    {
        semantic_varlist(node->child->brother->brother, field_list);
    }
}

void semantic_paramdec(TreeNode *node, field_list_t *field_list)
{
    assert(strcmp(node->tokenname, "ParamDec") == 0);
    type_t *type_specifier = semantic_specifier(node->child);
    if (type_specifier == NULL)
    {
        return;
    }

    symbol_t symbol;
    semantic_vardec(node->child, &symbol, type_specifier);
    field_list_check_push_back(field_list, &symbol);
}

void symbol_table_check_add(symbol_t *symbol)
{
    if (symbol_table_find_name(symbol) != NULL)
    {
        print_semantic_error(3, symbol->lineno, symbol->name);
    }
    else
    {
        symbol_table_add(symbol);
    }
}

void struct_table_check_add(type_struct_t *new_struct, int lineno)
{
    if (struct_table_find_name(new_struct->name) != NULL)
    {
        print_semantic_error(16, lineno, new_struct->name);
    }
    else
    {
        struct_table_push_back(new_struct);
    }
}

void field_list_check_push_back(field_list_t *field_list, symbol_t *symbol)
{
    field_node_t *itor;
    for (itor = field_list->start; itor != NULL; itor = itor->next)
    {
        if (strcmp(itor->name, symbol->name) == 0)
        {
            print_semantic_error(15, symbol->lineno, symbol->name);
            return;
        }
    }
    field_list_push_back(field_list, symbol->type, symbol->name);
}