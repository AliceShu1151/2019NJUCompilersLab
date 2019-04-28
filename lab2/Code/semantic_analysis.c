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

void semantic_fundec(TreeNode *node, type_t *type, symbol_t *symbol, field_list_t *field_list);
void semantic_varlist(TreeNode *node, field_list_t *field_list);
void semantic_paramdec(TreeNode *node, field_list_t *field_list);

void semantic_compst(TreeNode *node, type_t *rtn_type);
void semantic_stmtlist(TreeNode *node, type_t *rtn_type);
void semantic_stmt(TreeNode *node, type_t *rtn_type);
void semantic_args(TreeNode *node, type_list_t *type_list);
type_t *exp_type_check_float(TreeNode *node, int *is_left_value);

type_t *semantic_exp(TreeNode *node, int *is_left_value);
type_t *exp_type_check_int(TreeNode *node, int *is_left_value);
type_t *exp_type_check_float(TreeNode *node, int *is_left_value);
type_t *exp_type_check_var(TreeNode *node, int *is_left_value);
type_t *exp_type_check_func(TreeNode *node, TreeNode *args, int *is_left_value);
type_t *exp_type_check_unary_minus(TreeNode *node, int *is_left_value);
type_t *exp_type_check_unary_not(TreeNode *node, int *is_left_value);
type_t *exp_type_check_struct_dot(TreeNode *exp, TreeNode *dot, TreeNode *id, int *is_left_value);
type_t *exp_type_check_array(TreeNode *exp_1, TreeNode *exp_2, int *is_left_value);
type_t *exp_type_check_assign(TreeNode *exp_1, TreeNode *exp_2, int *is_left_value);
type_t *exp_type_check_binary_bit(TreeNode *exp_1, TreeNode *exp_2, int *is_left_value);
type_t *exp_type_check_binary_relop(TreeNode *exp_1, TreeNode *exp_2, int *is_left_value);
type_t *exp_type_check_binary_arith(TreeNode *exp_1, TreeNode *exp_2, int *is_left_value);

void symbol_table_check_add(symbol_t *symbol);
void symbol_table_check_add_func(symbol_t *symbol_func, int is_define);

void compst_env_init(symbol_t *symbol, field_list_t *param_list);

void struct_table_check_add(type_struct_t *new_struct, int lineno);

void field_list_check_push_back(field_list_t *field_list, symbol_t *symbol);

void param_list_check_add(symbol_t *symbol, field_list_t *param_list);

void semantic_analysis(TreeNode *root)
{
    //Print_Tree(root);
    init_struct_type_table();
    init_symbol_table();
    semantic_analysis_r(root);
    //print_struct_type_table();
    //print_symbol_table();
    //printf("fuck!\n");
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
        field_list_t *field_list = create_field_list();
        semantic_fundec(specifier_next, type_specifier, symbol, field_list);
        if (strcmp(specifier_next->brother->tokenname, "SEMI") == 0)
        {
            symbol_table_check_add_func(symbol, 0);
        }
        else if (strcmp(specifier_next->brother->tokenname, "CompSt") == 0)
        {
            symbol_table_check_add_func(symbol, 1);

            symbol_table_env_stack_push();
            compst_env_init(symbol, field_list);
            semantic_compst(specifier_next->brother, type_specifier);

            symbol_table_env_stack_pop();
        }
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
        field_list_t *field_list = create_field_list();
        if (strcmp(struct_next->brother->brother->tokenname, "RC") != 0)
        {
            semantic_deflist(struct_next->brother->brother, STRUCT_SPECIFIER, field_list);
        }
        TreeNode *struct_id = struct_next->child;
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
    else if (strcmp(struct_next->tokenname, "LC") == 0)
    {
        field_list_t *field_list = create_field_list();
        if (strcmp(struct_next->brother->tokenname, "RC") != 0)
        {
            semantic_deflist(struct_next->brother, STRUCT_SPECIFIER, field_list);
        }
        specifier_struct = (type_t *)create_type_struct(NULL, field_list);
        return specifier_struct;
    }
    return NULL;
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
    TreeNode *assignop = node->child->brother;
    if (assignop == NULL)
    {
        return;
    }
    if (strcmp(assignop->tokenname, "ASSIGNOP") == 0)
    {
        if (where == STRUCT_SPECIFIER)
        {
            print_semantic_error(15, node->lineno, symbol->name);
        }
        if (where == STATEMENT)
        {
            type_t *type_exp = semantic_exp(assignop->brother, NULL);
            if (type_is_equal(type, type_exp) == TYPE_NOT_EQUAL)
            {
                print_semantic_error(5, node->child->lineno);
            }
        }
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
            print_semantic_error(12, node->child->brother->brother->lineno,
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
    if (node->child->brother != NULL)
    {
        semantic_extdeclist(node->child->brother->brother, type);
    }
}

void semantic_fundec(TreeNode *node, type_t *type, symbol_t *symbol, field_list_t *field_list)
{
    assert(strcmp(node->tokenname, "FunDec") == 0);
    TreeNode *fundec_id = node->child;
    type_list_t *param_list = create_type_list();
    if (strcmp(fundec_id->brother->brother->tokenname, "RP") != 0)
    {
        semantic_varlist(fundec_id->brother->brother, field_list);
        field_list_add_to_type_list(field_list, param_list);
    }
    type_t *type_func = (type_t *)create_type_func(type, param_list);
    init_symbol(symbol, fundec_id->idname, type_func, fundec_id->lineno, NOT_DEFINED);
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
    semantic_vardec(node->child->brother, &symbol, type_specifier);
    field_list_check_push_back(field_list, &symbol);
}

void semantic_compst(TreeNode *node, type_t *rtn_type)
{
    assert(strcmp(node->tokenname, "CompSt") == 0);
    TreeNode *next = node->child->brother;
    if (strcmp(next->tokenname, "DefList") == 0)
    {
        field_list_t *field_list = create_field_list();
        semantic_deflist(next, STATEMENT, field_list);
        next = next->brother;
    }
    if (strcmp(next->tokenname, "StmtList") == 0)
    {
        semantic_stmtlist(next, rtn_type);
    }
}

void semantic_stmtlist(TreeNode *node, type_t *rtn_type)
{
    assert(strcmp(node->tokenname, "StmtList") == 0);
    semantic_stmt(node->child, rtn_type);
    if (node->child->brother != NULL)
    {
        semantic_stmtlist(node->child->brother, rtn_type);
    }
}

void semantic_stmt(TreeNode *node, type_t *rtn_type)
{
    assert(strcmp(node->tokenname, "Stmt") == 0);
    TreeNode *type_stmt = node->child;
    int is_left_value;
    if (strcmp(type_stmt->tokenname, "Exp") == 0)
    {
        semantic_exp(node->child, &is_left_value);
    }
    else if (strcmp(type_stmt->tokenname, "CompSt") == 0)
    {
        symbol_table_env_stack_push();
        semantic_compst(node->child, rtn_type);
        symbol_table_env_stack_pop();
    }
    else if (strcmp(type_stmt->tokenname, "RETURN") == 0)
    {
        type_t *rel_type = semantic_exp(type_stmt->brother, &is_left_value);
        if (rel_type != NULL && type_is_equal(rtn_type, rel_type) == TYPE_NOT_EQUAL)
        {
            print_semantic_error(8, type_stmt->brother->lineno);
        }
    }
    else if (strcmp(type_stmt->tokenname, "IF") == 0)
    {
        TreeNode *exp = type_stmt->brother->brother;
        semantic_exp(exp, NULL);
        TreeNode *stmt = exp->brother->brother;
        semantic_stmt(stmt, rtn_type);
        if (stmt->brother != NULL)
        {
            stmt = stmt->brother->brother;
            semantic_stmt(stmt, rtn_type);
        }
    }
    else if (strcmp(type_stmt->tokenname, "WHILE") == 0)
    {
        TreeNode *exp = type_stmt->brother->brother;
        type_t *rtn_type = semantic_exp(exp, &is_left_value);
        TreeNode *stmt = exp->brother->brother;
        semantic_stmt(stmt, rtn_type);
    }
}

void semantic_args(TreeNode *node, type_list_t *type_list)
{
    if (node == NULL)
    {
        return;
    }
    assert(strcmp(node->tokenname, "Args") == 0);
    TreeNode *exp = node->child;

    type_t *type_exp = semantic_exp(exp, NULL);
    if (type_exp == NULL)
        return;
    type_list_push_back(type_list, type_exp);
    if (exp->brother != NULL)
    {
        semantic_args(exp->brother->brother, type_list);
    }
}

type_t *semantic_exp(TreeNode *node, int *is_left_value)
{
    //printf("%d\n", node->lineno);
    TreeNode *child_1 = node->child;
    TreeNode *child_2, *child_3;
    assert(strcmp(node->tokenname, "Exp") == 0);
    if (strcmp(child_1->tokenname, "INT") == 0)
        return exp_type_check_int(child_1, is_left_value);
    else if (strcmp(child_1->tokenname, "FLOAT") == 0)
        return exp_type_check_float(child_1, is_left_value);
    else if (strcmp(child_1->tokenname, "ID") == 0)
    {
        if (child_1->brother == NULL)
            return exp_type_check_var(child_1, is_left_value);
        child_3 = child_1->brother->brother;
        if ((strcmp(child_3->tokenname, "RP") == 0))
            return exp_type_check_func(child_1, NULL, is_left_value);
        else if ((strcmp(child_3->tokenname, "Args") == 0))
            return exp_type_check_func(child_1, child_3, is_left_value);
    }
    else if (strcmp(child_1->tokenname, "LP") == 0)
        return semantic_exp(child_1->brother, is_left_value);
    else if (strcmp(child_1->tokenname, "MINUS") == 0)
        return exp_type_check_unary_minus(child_1, is_left_value);
    else if (strcmp(child_1->tokenname, "NOT") == 0)
        return exp_type_check_unary_not(child_1, is_left_value);
    else if (strcmp(child_1->tokenname, "Exp") == 0)
    {
        child_2 = child_1->brother;
        child_3 = child_2->brother;
        if (strcmp(child_2->tokenname, "DOT") == 0)
            return exp_type_check_struct_dot(child_1, child_2, child_3, is_left_value);
        else if (strcmp(child_2->tokenname, "LB") == 0)
            return exp_type_check_array(child_1, child_3, is_left_value);
        else if (strcmp(child_2->tokenname, "ASSIGNOP") == 0)
            return exp_type_check_assign(child_1, child_3, is_left_value);
        else if (strcmp(child_2->tokenname, "AND") == 0 || strcmp(child_2->tokenname, "OR") == 0)
            return exp_type_check_binary_bit(child_1, child_3, is_left_value);
        else if (strcmp(child_2->tokenname, "RELOP") == 0)
            return exp_type_check_binary_relop(child_1, child_3, is_left_value);
        else if (strcmp(child_2->tokenname, "PLUS") == 0 || strcmp(child_2->tokenname, "MINUS") == 0 ||
                 strcmp(child_2->tokenname, "STAR") == 0 || strcmp(child_2->tokenname, "DIV") == 0)
            return exp_type_check_binary_arith(child_1, child_3, is_left_value);
    }
    return NULL;
}

type_t *exp_type_check_int(TreeNode *node, int *is_left_value)
{
    assert(strcmp(node->tokenname, "INT") == 0);
    if (is_left_value != NULL)
    {
        *is_left_value = 0;
    }
    return (type_t *)create_type_basic(TYPE_BASIC_INT);
}

type_t *exp_type_check_float(TreeNode *node, int *is_left_value)
{
    assert(strcmp(node->tokenname, "FLOAT") == 0);
    if (is_left_value != NULL)
    {
        *is_left_value = 0;
    }
    return (type_t *)create_type_basic(TYPE_BASIC_FLOAT);
}

type_t *exp_type_check_var(TreeNode *node, int *is_left_value)
{
    assert(strcmp(node->tokenname, "ID") == 0);
    if (is_left_value != NULL)
    {
        *is_left_value = 1;
    }
    symbol_t *symbol = symbol_table_find_name(node->idname);
    if (symbol == NULL)
    {
        print_semantic_error(1, node->lineno, node->idname);
        return NULL;
    }
    return symbol->type;
}

type_t *exp_type_check_func(TreeNode *node, TreeNode *args, int *is_left_value)
{
    assert(strcmp(node->tokenname, "ID") == 0);
    if (is_left_value != NULL)
    {
        *is_left_value = 0;
    }
    symbol_t *symbol = symbol_table_find_name(node->idname);
    if (symbol == NULL)
    {
        print_semantic_error(2, node->lineno, node->idname);
        return NULL;
    }
    if (symbol->type->type_kind != TYPE_FUNCTION) 
    {        
        print_semantic_error(11, node->lineno, node->idname);
        return NULL;
    }
    type_func_t *type_func = (type_func_t *)symbol->type;
    type_list_t *arg_type_list = create_type_list();
    semantic_args(args, arg_type_list);
    if (type_type_list_is_equal(type_func->parameters, arg_type_list) == TYPE_NOT_EQUAL)
    {
        printf("Error type %d at Line %d: Function \"%s(", 9, node->lineno, symbol->name);
        print_type_list_simple(arg_type_list);
        printf(")\" is not applicable for arguments \"(");
        print_type_list_simple(type_func->parameters);
        printf(")\".\n");
    }
    return type_func->return_type;
}

type_t *exp_type_check_unary_minus(TreeNode *node, int *is_left_value)
{
    assert(strcmp(node->tokenname, "MINUS") == 0);
    if (is_left_value != NULL)
    {
        *is_left_value = 0;
    }
    type_t *type_exp = semantic_exp(node->brother, NULL);
    if (type_exp == NULL)
    {
        return NULL;
    }
    if (type_exp->type_kind != TYPE_BASIC)
    {
        print_semantic_error(7, node->child->lineno);
    }
    return type_exp;
}

type_t *exp_type_check_unary_not(TreeNode *node, int *is_left_value)
{
    assert(strcmp(node->tokenname, "NOT") == 0);
    if (is_left_value != NULL)
    {
        *is_left_value = 0;
    }
    type_t *type_exp = semantic_exp(node->brother, NULL);
    if (type_exp == NULL)
    {
        return NULL;
    }
    if (!type_is_int(type_exp))
    {
        print_semantic_error(7, node->brother->lineno);
        return NULL;
    }
    return type_exp;
}

type_t *exp_type_check_struct_dot(TreeNode *exp, TreeNode *dot, TreeNode *id, int *is_left_value)
{
    assert(strcmp(dot->tokenname, "DOT") == 0);
    if (is_left_value != NULL)
    {
        *is_left_value = 1;
    }
    type_t *type_exp = semantic_exp(exp, NULL);
    if (type_exp == NULL)
    {
        return NULL;
    }

    if (type_exp->type_kind != TYPE_STRUCT)
    {
        print_semantic_error(13, dot->lineno);
        return NULL;
    }
    field_node_t *field_node = field_list_find_name(((type_struct_t *)type_exp)->struct_fields, id->idname);
    if (field_node == NULL)
    {    
        print_semantic_error(14, exp->lineno, id->idname);
        return NULL;
    }
    return field_node->type;
}

type_t *exp_type_check_array(TreeNode *exp_1, TreeNode *exp_2, int *is_left_value)
{
    if (is_left_value != NULL)
    {
        *is_left_value = 1;
    }
    type_t *type_1 = semantic_exp(exp_1, NULL);
    type_t *type_2 = semantic_exp(exp_2, NULL);
    if (type_1 != NULL && type_1->type_kind != TYPE_ARRAY)
    {
        print_semantic_error(10, exp_1->lineno);
    }
    if (type_2 != NULL && !type_is_int(type_2))
    {
        print_semantic_error(12, exp_2->lineno);
    }
    if (type_1 != NULL && type_1->type_kind == TYPE_ARRAY)
    {
        return ((type_array_t *)type_1)->extend;
    }
    return NULL;
}

type_t *exp_type_check_assign(TreeNode *exp_1, TreeNode *exp_2, int *is_left_value)
{
    if (is_left_value != NULL)
    {
        *is_left_value = 0;
    }
    int type_1_is_left_value;
    type_t *type_1 = semantic_exp(exp_1, &type_1_is_left_value);
    type_t *type_2 = semantic_exp(exp_2, NULL);
    if (type_1 != NULL && !type_1_is_left_value)
    {
        print_semantic_error(6, exp_1->lineno);
        return NULL;
    }
    if (type_1 == NULL || type_2 == NULL)
    {
        return NULL;
    }
    if (type_is_equal(type_1, type_2) == TYPE_NOT_EQUAL)
    {
        print_semantic_error(5, exp_1->lineno);
        return NULL;
    }
    assert(type_1->type_kind != TYPE_FUNCTION);
    return type_1;
}

type_t *exp_type_check_binary_bit(TreeNode *exp_1, TreeNode *exp_2, int *is_left_value)
{
    if (is_left_value != NULL)
    {
        *is_left_value = 0;
    }
    type_t *type_1 = semantic_exp(exp_1, NULL);
    type_t *type_2 = semantic_exp(exp_2, NULL);
    if (type_1 == NULL || type_2 == NULL)
    {
        return NULL;
    }
    if (type_is_equal(type_1, type_2) == TYPE_NOT_EQUAL)
    {
        print_semantic_error(7, exp_1->lineno);
        return NULL;
    }
    if (!type_is_int(type_1))
    {
        print_semantic_error(7, exp_1->lineno);
        return NULL;
    }
    return type_1;
}

type_t *exp_type_check_binary_relop(TreeNode *exp_1, TreeNode *exp_2, int *is_left_value)
{
    if (is_left_value != NULL)
    {
        *is_left_value = 0;
    }
    type_t *type_1 = semantic_exp(exp_1, NULL);
    type_t *type_2 = semantic_exp(exp_2, NULL);
    if (type_1 == NULL || type_2 == NULL)
    {
        return NULL;
    }
    if (type_is_equal(type_1, type_2) == TYPE_NOT_EQUAL)
    {
        print_semantic_error(7, exp_1->lineno);
        return NULL;
    }
    if (type_1->type_kind != TYPE_BASIC)
    {
        print_semantic_error(7, exp_1->lineno);
        return NULL;
    }
    return (type_t *)create_type_basic(TYPE_BASIC_INT);
}

type_t *exp_type_check_binary_arith(TreeNode *exp_1, TreeNode *exp_2, int *is_left_value)
{
    if (is_left_value != NULL)
    {
        *is_left_value = 0;
    }
    type_t *type_1 = semantic_exp(exp_1, NULL);
    type_t *type_2 = semantic_exp(exp_2, NULL);
    if (type_1 == NULL || type_2 == NULL)
    {
        return NULL;
    }
    if (type_is_equal(type_1, type_2) == TYPE_NOT_EQUAL)
    {
        print_semantic_error(7, exp_1->lineno);
        return NULL;
    }
    if (type_1->type_kind != TYPE_BASIC)
    {
        print_semantic_error(7, exp_1->lineno);
        return NULL;
    }
    return type_1;
}

void compst_env_init(symbol_t *symbol, field_list_t *param_list)
{
    param_list_add_env_layer(param_list);
}

void symbol_table_check_add(symbol_t *symbol)
{
    if (struct_table_find_name(symbol->name))
    {
        print_semantic_error(3, symbol->lineno, symbol->name);
    }
    if (top_env_layer_find_name(symbol->name) != NULL)
    {
        print_semantic_error(3, symbol->lineno, symbol->name);
    }
    else
    {
        symbol_table_add(symbol);
    }
}

void symbol_table_check_add_func(symbol_t *symbol_func, int is_define)
{
    if (struct_table_find_name(symbol_func->name) != NULL)
    {
        print_semantic_error(3, symbol_func->lineno, symbol_func->name);
        return;
    }
    symbol_t *find_in_table = symbol_table_find_name(symbol_func->name);
    if (find_in_table != NULL)
    {
        if (find_in_table->is_defined == DEFINED && is_define == DEFINED)
        {
            print_semantic_error(4, symbol_func->lineno, symbol_func->name);
            return;
        }
        if (type_is_equal(symbol_func->type, find_in_table->type) == TYPE_NOT_EQUAL)
        {
            print_semantic_error(19, symbol_func->lineno, symbol_func->name);
            return;
        }
        if (is_define)
        {
            find_in_table->is_defined = DEFINED;
        }
    }
    else
    {
        symbol_func->is_defined = is_define;
        symbol_table_add(symbol_func);
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
    field_list_push_back(field_list, symbol->type, symbol->name, symbol->lineno);
}