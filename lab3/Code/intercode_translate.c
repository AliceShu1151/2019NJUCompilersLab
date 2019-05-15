#include "intercode_translate.h"

void intercode_translate_r(TreeNode *node);

void translate_extdef(TreeNode *node);
void translate_extdeclist(TreeNode *node, type_t *type);
void translate_deflist(TreeNode *node, int where, field_list_t *field_list);
void translate_declist(TreeNode *node, type_t *type, field_list_t *field_list, int where);
void translate_dec(TreeNode *node, type_t *type, field_list_t *field_list, int where);
void translate_vardec(TreeNode *node, symbol_t *symbol, type_t *type);

void translate_params(field_list_t *field_list);
void translate_compst(TreeNode *node);
void translate_stmtlist(TreeNode *node);
void translate_stmt(TreeNode *node);
operand_t *translate_args(TreeNode *node);

operand_t *translate_exp(TreeNode *node);
operand_t *translate_exp_int(TreeNode *node);
operand_t *translate_exp_float(TreeNode *node);
operand_t *translate_exp_var(TreeNode *node);
operand_t *translate_exp_func(TreeNode *node, TreeNode *args);
operand_t *translate_exp_unary_minus(TreeNode *node);
operand_t *translate_exp_unary_not(TreeNode *node);
operand_t *translate_addrexp(TreeNode *exp);
operand_t *translate_array_struct(TreeNode *node, type_t **type);
operand_t *translate_exp_struct(TreeNode *exp_1, TreeNode *exp_2, type_t **type);
operand_t *translate_exp_array(TreeNode *exp_1, TreeNode *exp_2, type_t **type);
operand_t *translate_exp_assign(TreeNode *exp_1, TreeNode *exp_2);
operand_t *translate_exp_binary_arith(const char *operator_name, TreeNode *exp_1, TreeNode *exp_2);
operand_t *translate_boolexp(TreeNode *boolexp);

void translate_cond(TreeNode *node, int label_true, int label_false);
operand_t *dref_address(operand_t *address);

void intercode_translate(TreeNode *root)
{
    init_struct_type_table();
    init_symbol_table();
    init_intercode_list();
    intercode_translate_r(root);
    print_intercode_list();
}

void intercode_translate_r(TreeNode *node)
{
    if (node == NULL)
    {
        return;
    }

    if (strcmp(node->tokenname, "ExtDef") == 0)
    {
        translate_extdef(node);
        return;
    }

    for (TreeNode *child = node->child; child != NULL; child = child->brother)
    {
        intercode_translate_r(child);
    }
}

void translate_extdef(TreeNode *node)
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
        translate_extdeclist(specifier_next, type_specifier);
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
            int symbol_is_add = symbol_table_check_add_func(symbol, 1);
            if (symbol_is_add)
            {
                intercode_node_t *func = create_intercode_node_func(symbol->name);
                intercode_list_push_back(func);
            }

            symbol_table_env_stack_push();
            compst_env_init(symbol, field_list);
            translate_params(field_list);
            translate_compst(specifier_next->brother);

            symbol_table_env_stack_pop();
        }
    }
}

void translate_params(field_list_t *field_list)
{
    if (field_list->size == 0)
    {
        return;
    }
    field_node_t *itor;
    for (itor = field_list->start; itor != NULL; itor = itor->next)
    {
        intercode_node_t *param = create_intercode_node_param(symbol_table_find_name(itor->name)->var_no);
        intercode_list_push_back(param);
    }
}

void translate_extdeclist(TreeNode *node, type_t *type)
{
    assert(strcmp(node->tokenname, "ExtDecList") == 0);

    symbol_t *symbol = create_symbol();
    semantic_vardec(node->child, symbol, type);
    int symbol_is_add = symbol_table_check_add(symbol);
    if (symbol_is_add)
    {
        intercode_node_t *extdec = create_intercode_node_dec(symbol->var_no, sizeof_type(type));
        intercode_list_push_back(extdec);
    }
    if (node->child->brother != NULL)
    {
        translate_extdeclist(node->child->brother->brother, type);
    }
}

void translate_compst(TreeNode *node)
{
    assert(strcmp(node->tokenname, "CompSt") == 0);
    TreeNode *next = node->child->brother;
    if (strcmp(next->tokenname, "DefList") == 0)
    {
        field_list_t *field_list = create_field_list();
        translate_deflist(next, STATEMENT, field_list);
        next = next->brother;
    }
    if (strcmp(next->tokenname, "StmtList") == 0)
    {
        translate_stmtlist(next);
    }
}

void translate_deflist(TreeNode *node, int where, field_list_t *field_list)
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
            translate_declist(def->child->brother, type_specifier, field_list, where);
        }
        deflist = def->brother;
    }
}

void translate_declist(TreeNode *node, type_t *type, field_list_t *field_list, int where)
{
    assert(strcmp(node->tokenname, "DecList") == 0);

    //printf("DecList\n");
    TreeNode *declist = node;
    while (declist != NULL)
    {
        TreeNode *dec = declist->child;
        translate_dec(dec, type, field_list, where);
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

void translate_dec(TreeNode *node, type_t *type, field_list_t *field_list, int where)
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
        int symbol_is_add = symbol_table_check_add(symbol);
        if (symbol_is_add && (symbol->type->type_kind == TYPE_ARRAY || symbol->type->type_kind == TYPE_STRUCT))
        {
            intercode_node_t *dec = create_intercode_node_dec(symbol->var_no, sizeof_type(symbol->type));
            intercode_list_push_back(dec);
        }
    }
    TreeNode *assignop = node->child->brother;
    if (assignop == NULL)
    {
        return;
    }
    if (strcmp(assignop->tokenname, "ASSIGNOP") == 0)
    {
        operand_t *left = create_operand_var(OPERAND_VARIABLE_V, symbol->var_no);
        operand_t *right = translate_exp(assignop->brother);
        right = dref_address(right);
        assert(left->kind != OPERAND_ADDRESS_T && left->kind != OPERAND_ADDRESS_V);
        intercode_node_t *assign = create_intercode_node_assign(left, right);
        intercode_list_push_back(assign);
    }
}

void translate_stmtlist(TreeNode *node)
{
    assert(strcmp(node->tokenname, "StmtList") == 0);
    translate_stmt(node->child);
    if (node->child->brother != NULL)
    {
        translate_stmtlist(node->child->brother);
    }
}

void translate_stmt(TreeNode *node)
{
    assert(strcmp(node->tokenname, "Stmt") == 0);
    TreeNode *type_stmt = node->child;
    if (strcmp(type_stmt->tokenname, "Exp") == 0)
    {
        translate_exp(node->child);
    }
    else if (strcmp(type_stmt->tokenname, "CompSt") == 0)
    {
        symbol_table_env_stack_push();
        translate_compst(node->child);
        symbol_table_env_stack_pop();
    }
    else if (strcmp(type_stmt->tokenname, "RETURN") == 0)
    {
        operand_t *ret = translate_exp(type_stmt->brother);
        ret = dref_address(ret);
        intercode_node_t *rtn = create_intercode_node_return(ret);
        intercode_list_push_back(rtn);
    }
    else if (strcmp(type_stmt->tokenname, "IF") == 0)
    {
        int label_1 = malloc_label_no();
        TreeNode *exp = type_stmt->brother->brother;
        translate_cond(exp, FALL, label_1);

        TreeNode *stmt = exp->brother->brother;
        translate_stmt(stmt);
        if (stmt->brother == NULL)
        {
            intercode_node_t *label_flase = create_intercode_node_label(label_1);
            intercode_list_push_back(label_flase);
        }
        if (stmt->brother != NULL)
        {
            int label_3 = malloc_label_no();
            intercode_node_t *goto_else = create_intercode_node_goto(label_3);
            intercode_list_push_back(goto_else);
            intercode_node_t *label_flase = create_intercode_node_label(label_1);
            intercode_list_push_back(label_flase);

            stmt = stmt->brother->brother;
            translate_stmt(stmt);
            intercode_node_t *label_else = create_intercode_node_label(label_3);
            intercode_list_push_back(label_else);
        }
    }
    else if (strcmp(type_stmt->tokenname, "WHILE") == 0)
    {
        int label_1 = malloc_label_no();
        int label_2 = malloc_label_no();

        intercode_node_t *label_while = create_intercode_node_label(label_1);
        intercode_list_push_back(label_while);

        TreeNode *exp = type_stmt->brother->brother;
        translate_cond(exp, FALL, label_2);

        TreeNode *stmt = exp->brother->brother;
        translate_stmt(stmt);

        intercode_node_t *goto_while = create_intercode_node_goto(label_1);
        intercode_list_push_back(goto_while);

        intercode_node_t *label_out = create_intercode_node_label(label_2);
        intercode_list_push_back(label_out);
    }
}

operand_t *translate_args(TreeNode *node)
{
    if (node == NULL)
    {
        return NULL;
    }
    assert(strcmp(node->tokenname, "Args") == 0);
    TreeNode *exp = node->child;

    operand_t *exp_operand = translate_exp(exp);
    exp_operand = dref_address(exp_operand);
    if (exp_operand == NULL)
        return NULL;
    if (exp->brother != NULL)
    {
        translate_args(exp->brother->brother);
    }
    intercode_node_t *arg = create_intercode_node_arg(exp_operand->kind, exp_operand);
    intercode_list_push_back(arg);
    return exp_operand;
}

operand_t *translate_exp(TreeNode *node)
{
    //printf("%d\n", node->lineno);
    TreeNode *child_1 = node->child;
    TreeNode *child_2, *child_3;
    assert(strcmp(node->tokenname, "Exp") == 0);
    if (strcmp(child_1->tokenname, "INT") == 0)
        return translate_exp_int(child_1);
    else if (strcmp(child_1->tokenname, "FLOAT") == 0)
        return translate_exp_float(child_1);
    else if (strcmp(child_1->tokenname, "ID") == 0)
    {
        if (child_1->brother == NULL)
            return translate_exp_var(child_1);
        child_3 = child_1->brother->brother;
        if ((strcmp(child_3->tokenname, "RP") == 0))
            return translate_exp_func(child_1, NULL);
        else if ((strcmp(child_3->tokenname, "Args") == 0))
            return translate_exp_func(child_1, child_3);
    }
    else if (strcmp(child_1->tokenname, "LP") == 0)
        return translate_exp(child_1->brother);
    else if (strcmp(child_1->tokenname, "MINUS") == 0)
        return translate_exp_unary_minus(child_1);
    else if (strcmp(child_1->tokenname, "NOT") == 0)
        return translate_boolexp(node);
    else if (strcmp(child_1->tokenname, "Exp") == 0)
    {
        child_2 = child_1->brother;
        child_3 = child_2->brother;
        if (strcmp(child_2->tokenname, "DOT") == 0)
            return translate_addrexp(node);
        else if (strcmp(child_2->tokenname, "LB") == 0)
            return translate_addrexp(node);
        else if (strcmp(child_2->tokenname, "ASSIGNOP") == 0)
            return translate_exp_assign(child_1, child_3);
        else if (strcmp(child_2->tokenname, "AND") == 0 || strcmp(child_2->tokenname, "OR") == 0 ||
                 strcmp(child_2->tokenname, "RELOP") == 0)
            return translate_boolexp(node);
        else if (strcmp(child_2->tokenname, "PLUS") == 0 || strcmp(child_2->tokenname, "MINUS") == 0 ||
                 strcmp(child_2->tokenname, "STAR") == 0 || strcmp(child_2->tokenname, "DIV") == 0)
            return translate_exp_binary_arith(child_2->tokenname, child_1, child_3);
    }
    assert(0);
    return NULL;
}

operand_t *translate_exp_int(TreeNode *node)
{
    assert(strcmp(node->tokenname, "INT") == 0);
    return create_operand_const(node);
}

operand_t *translate_exp_float(TreeNode *node)
{
    assert(strcmp(node->tokenname, "FLOAT") == 0);
    return create_operand_const(node);
}

operand_t *translate_exp_var(TreeNode *node)
{
    assert(strcmp(node->tokenname, "ID") == 0);
    symbol_t *symbol = symbol_table_find_name(node->idname);

    if ((symbol->type->type_kind == TYPE_ARRAY || symbol->type->type_kind == TYPE_STRUCT) &&
        !symbol->is_param)
    {
        operand_t *addr = create_operand_var(OPERAND_ADDRESS_T, malloc_var_no());
        intercode_node_t *ref = create_intercode_node_ref(addr, create_operand_var(OPERAND_VARIABLE_V, symbol->var_no));
        intercode_list_push_back(ref);
        return addr;
    }
    return create_operand_var(OPERAND_VARIABLE_V, symbol->var_no);
    ;
}

operand_t *translate_exp_func(TreeNode *node, TreeNode *args)
{
    assert(strcmp(node->tokenname, "ID") == 0);
    symbol_t *symbol = symbol_table_find_name(node->idname);
    operand_t *var = translate_args(args);
    operand_t *ret = create_operand_var(OPERAND_VARIABLE_T, malloc_var_no());
    if (strcmp(node->idname, "write") == 0)
    {
        intercode_node_t *write = create_intercode_node_write(var);
        intercode_list_push_back(write);
        return var;
    }
    else if (strcmp(node->idname, "read") == 0)
    {
        intercode_node_t *read = create_intercode_node_read(ret);
        intercode_list_push_back(read);
        return ret;
    }
    intercode_node_t *call = create_intercode_node_call(symbol->name, ret);
    intercode_list_push_back(call);

    return ret;
}

operand_t *translate_exp_unary_minus(TreeNode *node)
{
    assert(strcmp(node->tokenname, "MINUS") == 0);
    operand_t *exp_operand = translate_exp(node->brother);
    exp_operand = dref_address(exp_operand);
    if (exp_operand->kind == OPERAND_CONSTANT_I)
    {
        exp_operand->value_int = 0 - exp_operand->value_int;
        return exp_operand;
    }
    if (exp_operand->kind == OPERAND_CONSTANT_F)
    {
        exp_operand->value_flt = 0 - exp_operand->value_flt;
        return exp_operand;
    }
    operand_t *zero = create_operand_const_int(0);
    operand_t *target = create_operand_var(OPERAND_VARIABLE_T, malloc_var_no());
    intercode_node_t *minus = create_intercode_node_binary(OPERATOR_MIN, target, zero, exp_operand);
    intercode_list_push_back(minus);
    return target;
}

operand_t *translate_boolexp(TreeNode *boolexp)
{
    int label_1 = malloc_label_no();
    operand_t *place = create_operand_var(OPERAND_VARIABLE_T, malloc_var_no());
    operand_t *zero = create_operand_const_int(0);
    operand_t *one = create_operand_const_int(1);
    intercode_node_t *place_0 = create_intercode_node_assign(place, zero);
    intercode_list_push_back(place_0);
    translate_cond(boolexp, FALL, label_1);
    intercode_node_t *place_1 = create_intercode_node_assign(place, one);
    intercode_list_push_back(place_1);
    intercode_node_t *label_false = create_intercode_node_label(label_1);
    intercode_list_push_back(label_false);
    return place;
}

void translate_cond(TreeNode *node, int label_true, int label_false)
{
    TreeNode *child_1 = node->child;
    TreeNode *child_2, *child_3;

    if (strcmp(child_1->tokenname, "NOT") == 0)
    {
        translate_cond(child_1->brother, label_false, label_true);
        return;
    }
    if (strcmp(child_1->tokenname, "Exp") == 0)
    {
        child_2 = child_1->brother;
        child_3 = child_2->brother;
        if (strcmp(child_2->tokenname, "AND") == 0)
        {
            int label_1;
            if (label_false == FALL)
                label_1 = malloc_label_no();
            else
                label_1 = label_false;
            translate_cond(child_1, FALL, label_1);
            translate_cond(child_3, label_true, label_false);
            if (label_false == FALL)
            {
                intercode_node_t *label = create_intercode_node_label(label_1);
                intercode_list_push_back(label);
            }
            return;
        }
        if (strcmp(child_2->tokenname, "OR") == 0)
        {
            int label_1;
            if (label_true != FALL)
                label_1 = label_true;
            else
                label_1 = malloc_label_no();
            translate_cond(child_1, label_1, FALL);
            translate_cond(child_3, label_true, label_false);
            if (label_true == FALL)
            {
                intercode_node_t *label = create_intercode_node_label(label_1);
                intercode_list_push_back(label);
            }
            return;
        }
        if (strcmp(child_2->tokenname, "RELOP") == 0)
        {
            operand_t *exp_1 = translate_exp(child_1);
            exp_1 = dref_address(exp_1);
            operand_t *exp_2 = translate_exp(child_3);
            exp_2 = dref_address(exp_2);
            if (label_true != FALL && label_false != FALL)
            {
                intercode_node_t *relop = create_intercode_node_if_goto(exp_1, child_2->relop, exp_2, label_true);
                intercode_list_push_back(relop);
                intercode_node_t *goto_false = create_intercode_node_goto(label_false);
                intercode_list_push_back(goto_false);
            }
            else if (label_true != FALL)
            {
                intercode_node_t *relop = create_intercode_node_if_goto(exp_1, child_2->relop, exp_2, label_true);
                intercode_list_push_back(relop);
            }
            else if (label_false != FALL)
            {
                intercode_node_t *relop = create_intercode_node_if_goto(exp_1, relop_not(child_2->relop), exp_2, label_false);
                intercode_list_push_back(relop);
            }
            return;
        }
    }
    operand_t *cond = translate_exp(node);
    cond = dref_address(cond);
    operand_t *zero = create_operand_const_int(0);

    if (label_true != FALL && label_false != FALL)
    {
        intercode_node_t *relop = create_intercode_node_if_goto(cond, "!=", zero, label_true);
        intercode_list_push_back(relop);
        intercode_node_t *goto_false = create_intercode_node_goto(label_false);
        intercode_list_push_back(goto_false);
    }
    else if (label_true != FALL)
    {
        intercode_node_t *relop = create_intercode_node_if_goto(cond, "!=", zero, label_true);
        intercode_list_push_back(relop);
    }
    else if (label_false != FALL)
    {
        intercode_node_t *relop = create_intercode_node_if_goto(cond, "==", zero, label_false);
        intercode_list_push_back(relop);
    }
}

operand_t *translate_addrexp(TreeNode *addrexp)
{
    return translate_array_struct(addrexp, NULL);
}

operand_t *translate_array_struct(TreeNode *node, type_t **type)
{
    TreeNode *child_1 = node->child;
    TreeNode *child_2, *child_3;
    if (strcmp(child_1->tokenname, "Exp") == 0)
    {
        child_2 = child_1->brother;
        child_3 = child_2->brother;
        if (strcmp(child_2->tokenname, "DOT") == 0)
            return translate_exp_struct(child_1, child_3, type);
        else if (strcmp(child_2->tokenname, "LB") == 0)
            return translate_exp_array(child_1, child_3, type);
    }
    else if (strcmp(child_1->tokenname, "ID") == 0)
    {
        symbol_t *symbol = symbol_table_find_name(child_1->idname);
        *type = symbol->type;
        operand_t *addr;
        if (symbol->is_param)
            addr = create_operand_var(OPERAND_ADDRESS_V, symbol->var_no);
        else
        {
            addr = create_operand_var(OPERAND_ADDRESS_T, malloc_var_no());
            intercode_node_t *ref = create_intercode_node_ref(addr, create_operand_var(OPERAND_VARIABLE_V, symbol->var_no));
            intercode_list_push_back(ref);
        }
        return addr;
    }
    assert(0);
    return NULL;
}

operand_t *translate_exp_array(TreeNode *exp_1, TreeNode *exp_2, type_t **rettype)
{
    type_t *type, *subtype;
    operand_t *left = translate_array_struct(exp_1, &type);
    operand_t *right = translate_exp(exp_2);
    operand_t *dref_right = dref_address(right);
    operand_t *target = create_operand_var(OPERAND_ADDRESS_T, malloc_var_no());
    assert(type->type_kind == TYPE_ARRAY);
    subtype = ((type_array_t *)type)->extend;
    if (rettype)
    {
        *rettype = subtype;
    }

    operand_t *offset = create_operand_var(OPERAND_VARIABLE_T, malloc_var_no());

    operand_t *size = create_operand_const_int(sizeof_type(subtype));
    intercode_node_t *star = create_intercode_node_binary(OPERATOR_STAR, offset, dref_right, size);
    intercode_list_push_back(star);

    intercode_node_t *plus = create_intercode_node_binary(OPERATOR_PLS, target, left, offset);
    intercode_list_push_back(plus);

    return target;
}

operand_t *translate_exp_struct(TreeNode *exp_1, TreeNode *exp_2, type_t **rettype)
{
    type_t *type;
    operand_t *left = translate_array_struct(exp_1, &type);
    assert(type->type_kind == TYPE_STRUCT);
    int offset_size = 0;
    for (field_node_t *itor = ((type_struct_t *)type)->struct_fields->start; itor != NULL; itor = itor->next)
    {
        if (strcmp(itor->name, exp_2->idname) == 0)
        {
            if (rettype)
            {
                *rettype = itor->type;
            }
            break;
        }
        offset_size += sizeof_type(itor->type);
    }

    operand_t *target = create_operand_var(OPERAND_ADDRESS_T, malloc_var_no());

    operand_t *offset = create_operand_const_int(offset_size);

    intercode_node_t *plus = create_intercode_node_binary(OPERATOR_PLS, target, left, offset);
    intercode_list_push_back(plus);

    return target;
}

operand_t *translate_exp_assign(TreeNode *exp_1, TreeNode *exp_2)
{
    operand_t *left = translate_exp(exp_1);
    operand_t *right = translate_exp(exp_2);
    right = dref_address(right);
    intercode_node_t *assign;
    if (left->kind == OPERAND_ADDRESS_T || left->kind == OPERAND_ADDRESS_V)
    {
        assign = create_intercode_node_dref_assign(left, right);
        left->kind = OPERAND_VARIABLE_T;
    }
    else
        assign = create_intercode_node_assign(left, right);
    intercode_list_push_back(assign);
    return left;
}

operand_t *translate_exp_binary_arith(const char *operator_name, TreeNode *exp_1, TreeNode *exp_2)
{
    operand_t *left = translate_exp(exp_1);
    left = dref_address(left);
    operand_t *right = translate_exp(exp_2);
    right = dref_address(right);

    operand_t *target = create_operand_var(OPERAND_VARIABLE_T, malloc_var_no());
    int operator_kind;
    if (strcmp(operator_name, "PLUS") == 0)
        operator_kind = OPERATOR_PLS;
    else if (strcmp(operator_name, "MINUS") == 0)
        operator_kind = OPERATOR_MIN;
    else if (strcmp(operator_name, "STAR") == 0)
        operator_kind = OPERATOR_STAR;
    else if (strcmp(operator_name, "DIV") == 0)
        operator_kind = OPERATOR_DIV;
    intercode_node_t *binary = create_intercode_node_binary(operator_kind, target, left, right);
    intercode_list_push_back(binary);
    return target;
}

operand_t *dref_address(operand_t *address)
{
    operand_t *dref_right;
    if (address->kind == OPERAND_ADDRESS_T || address->kind == OPERAND_ADDRESS_V)
    {
        dref_right = create_operand_var(OPERAND_VARIABLE_T, malloc_var_no());
        intercode_node_t *dref = create_intercode_node_dref(dref_right, address);
        intercode_list_push_back(dref);
    }
    else
    {
        dref_right = address;
    }
    return dref_right;
}