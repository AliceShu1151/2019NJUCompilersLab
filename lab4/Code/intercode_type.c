#include "intercode_type.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static intercode_list_t intercode_list;


operand_t *create_operand_const_int(int ival)
{
    operand_t *rtn = malloc(sizeof(operand_t));
    rtn->kind = OPERAND_CONSTANT_I;
    rtn->value_int = ival;
    return rtn;
}

operand_t *create_operand_const_float(float fval)
{
    operand_t *rtn = malloc(sizeof(operand_t));
    rtn->kind = OPERAND_CONSTANT_F;
    rtn->value_flt = fval;
    return rtn;
}

operand_t *create_operand_const(TreeNode *node)
{
    if (strcmp(node->tokenname, "INT") == 0)
        return create_operand_const_int(node->ival);
    else if (strcmp(node->tokenname, "FLOAT") == 0)    
        return create_operand_const_float(node->fval);
    return NULL;
}

operand_t *create_operand_var(int operand_kind, int var_no)
{
    operand_t *rtn = malloc(sizeof(operand_t));
    rtn->kind = operand_kind;
    rtn->var_no = var_no;
    return rtn;
}

void print_operand(operand_t *operand)
{
    assert(operand != NULL);
    if (operand->kind == OPERAND_VARIABLE_T)
        printf("t%d", operand->var_no);
    else if (operand->kind == OPERAND_VARIABLE_V)
        printf("v%d", operand->var_no);
    else if (operand->kind == OPERAND_CONSTANT_I)
        printf("#%d", operand->value_int);
    else if (operand->kind == OPERAND_CONSTANT_F)
        printf("#%f", operand->value_flt);
    else if (operand->kind == OPERAND_ADDRESS_T)
        printf("t%d", operand->var_no);
    else if (operand->kind == OPERAND_ADDRESS_V)
        printf("v%d", operand->var_no);
}

void print_operator(int operator_kind)
{
    if (operator_kind == OPERATOR_EQ)
        printf(" == ");
    else if (operator_kind == OPERATOR_NEQ)
        printf(" != ");
    else if (operator_kind == OPERATOR_G)
        printf(" > ");
    else if (operator_kind == OPERATOR_GE)
        printf(" >= ");
    else if (operator_kind == OPERATOR_L)
        printf(" < ");
    else if (operator_kind == OPERATOR_LE)
        printf(" <= ");
    else if (operator_kind == OPERATOR_PLS)
        printf(" + ");
    else if (operator_kind == OPERATOR_MIN)
        printf(" - ");
    else if (operator_kind == OPERATOR_STAR)
        printf(" * ");
    else if (operator_kind == OPERATOR_DIV)
        printf(" / ");
}

intercode_node_t *create_intercode_node_label(int label)
{
    intercode_node_label_t *rtn = malloc(sizeof(intercode_node_label_t));
    rtn->kind = CODE_LABEL;
    rtn->label = label;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_node_func(const char *func_name)
{
    intercode_node_func_t *rtn = malloc(sizeof(intercode_node_func_t));
    rtn->kind = CODE_FUNC;
    rtn->func_name = func_name;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_node_assign(operand_t *left, operand_t *right)
{
    intercode_node_assign_t *rtn = malloc(sizeof(intercode_node_assign_t));
    rtn->kind = CODE_ASSIGN;
    rtn->left = left;
    rtn->right = right;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_node_binary(int operator_kind, operand_t *target, operand_t *left, operand_t *right)
{
    intercode_node_binary_t *rtn = malloc(sizeof(intercode_node_binary_t));
    rtn->kind = CODE_OPERATOR;
    rtn->operator_kind = operator_kind;
    rtn->target = target;
    rtn->left = left;
    rtn->right = right;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_node_ref(operand_t *left, operand_t *right)
{
    intercode_node_ref_t *rtn = malloc(sizeof(intercode_node_ref_t));
    rtn->kind = CODE_REF;
    rtn->left = left;
    rtn->right = right;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_node_dref(operand_t *left, operand_t *right)
{
    intercode_node_dref_t *rtn = malloc(sizeof(intercode_node_dref_t));
    rtn->kind = CODE_DREF;
    rtn->left = left;
    rtn->right = right;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_node_dref_assign(operand_t *left, operand_t *right)
{
    intercode_node_dref_assign_t *rtn = malloc(sizeof(intercode_node_dref_assign_t));
    rtn->kind = CODE_DREF_ASSIGN;
    rtn->left = left;
    rtn->right = right;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_node_goto(int label)
{
    intercode_node_goto_t *rtn = malloc(sizeof(intercode_node_goto_t));
    rtn->kind = CODE_GOTO;
    rtn->label = label;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_node_if_goto(operand_t *if_left, const char *relop, operand_t *if_right, int target_label)
{
    intercode_node_if_goto_t *rtn = malloc(sizeof(intercode_node_if_goto_t));
    rtn->kind = CODE_IF_GOTO;
    rtn->if_left = if_left;
    rtn->relop = relop;
    rtn->if_right = if_right;
    rtn->target_label = target_label;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_node_return(operand_t *ret)
{
    intercode_node_return_t *rtn = malloc(sizeof(intercode_node_return_t));
    rtn->kind = CODE_RETURN;
    rtn->ret = ret;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_node_dec(int var_no, int size)
{
    intercode_node_dec_t *rtn = malloc(sizeof(intercode_node_dec_t));
    rtn->kind = CODE_DEC;
    rtn->var_no = var_no;
    rtn->size = size;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_node_arg(int operand_kind, operand_t *arg)
{
    intercode_node_arg_t *rtn = malloc(sizeof(intercode_node_arg_t));
    rtn->kind = CODE_ARG;
    rtn->operand_kind = operand_kind;
    rtn->arg = arg;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_node_call(const char *call_name, operand_t *call_rtn)
{
    intercode_node_call_t *rtn = malloc(sizeof(intercode_node_call_t));
    rtn->kind = CODE_CALL;
    rtn->call_name = call_name;
    rtn->call_rtn = call_rtn;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_node_param(int var_no)
{
    intercode_node_param_t *rtn = malloc(sizeof(intercode_node_param_t));
    rtn->kind = CODE_PARAM;
    rtn->var_no = var_no;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_node_read(operand_t *var)
{
    intercode_node_read_t *rtn = malloc(sizeof(intercode_node_read_t));
    rtn->kind = CODE_READ;
    rtn->var = var;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_node_write(operand_t *var)
{
    intercode_node_write_t *rtn = malloc(sizeof(intercode_node_write_t));
    rtn->kind = CODE_WRITE;
    rtn->var = var;
    return (intercode_node_t *)rtn;
}

void print_intercode_node(intercode_node_t *node)
{
    assert(node != NULL);
    if (node->kind == CODE_LABEL)
        print_intercode_node_label((intercode_node_label_t *)node);
    else if (node->kind == CODE_FUNC)
        print_intercode_node_func((intercode_node_func_t *)node);
    else if (node->kind == CODE_ASSIGN)
        print_intercode_node_assign((intercode_node_assign_t *)node);
    else if (node->kind == CODE_OPERATOR)
        print_intercode_node_binary((intercode_node_binary_t *)node);
    else if (node->kind == CODE_REF)
        print_intercode_node_ref((intercode_node_ref_t *)node);
    else if (node->kind == CODE_DREF)
        print_intercode_node_dref((intercode_node_dref_t *)node);
    else if (node->kind == CODE_DREF_ASSIGN)
        print_intercode_node_dref_assign((intercode_node_dref_assign_t *)node);
    else if (node->kind == CODE_GOTO)
        print_intercode_node_goto((intercode_node_goto_t *)node);
    else if (node->kind == CODE_IF_GOTO)
        print_intercode_node_if_goto((intercode_node_if_goto_t *)node);
    else if (node->kind == CODE_RETURN)
        print_intercode_node_return((intercode_node_return_t *)node);
    else if (node->kind == CODE_DEC)
        print_intercode_node_dec((intercode_node_dec_t *)node);
    else if (node->kind == CODE_ARG)
        print_intercode_node_arg((intercode_node_arg_t *)node);
    else if (node->kind == CODE_CALL)
        print_intercode_node_call((intercode_node_call_t *)node);
    else if (node->kind == CODE_PARAM)
        print_intercode_node_param((intercode_node_param_t *)node);
    else if (node->kind == CODE_READ)
        print_intercode_node_read((intercode_node_read_t *)node);
    else if (node->kind == CODE_WRITE)
        print_intercode_node_write((intercode_node_write_t *)node);
}

void print_intercode_node_label(intercode_node_label_t *node)
{
    printf("LABEL L%d : ", node->label);
}

void print_intercode_node_func(intercode_node_func_t *node)
{
    printf("FUNCTION %s : ", node->func_name);
}

void print_intercode_node_assign(intercode_node_assign_t *node)
{
    print_operand(node->left);
    printf(" := ");
    print_operand(node->right);
}

void print_intercode_node_binary(intercode_node_binary_t *node)
{
    print_operand(node->target);
    printf(" := ");
    print_operand(node->left);
    print_operator(node->operator_kind);
    print_operand(node->right);
}

void print_intercode_node_ref(intercode_node_ref_t *node)
{
    print_operand(node->left);
    printf(" := &");
    print_operand(node->right);
}

void print_intercode_node_dref(intercode_node_dref_t *node)
{
    print_operand(node->left);
    printf(" := *");
    print_operand(node->right);
}

void print_intercode_node_dref_assign(intercode_node_dref_assign_t *node)
{
    printf("*");
    print_operand(node->left);
    printf(" := ");
    print_operand(node->right);
}

void print_intercode_node_goto(intercode_node_goto_t *node)
{
    printf("GOTO L%d", node->label);
}

void print_intercode_node_if_goto(intercode_node_if_goto_t *node)
{
    printf("IF ");
    print_operand(node->if_left);
    printf(" %s ", node->relop);
    print_operand(node->if_right);
    printf(" GOTO L%d", node->target_label);
}

void print_intercode_node_return(intercode_node_return_t *node)
{
    printf("RETURN ");
    print_operand(node->ret);
}

void print_intercode_node_dec(intercode_node_dec_t *node)
{
    printf("DEC v%d %d", node->var_no, node->size);
}

void print_intercode_node_arg(intercode_node_arg_t *node)
{
    printf("ARG ");
    print_operand(node->arg);
}

void print_intercode_node_call(intercode_node_call_t *node)
{
    print_operand(node->call_rtn);
    printf(" := CALL %s", node->call_name);
}

void print_intercode_node_param(intercode_node_param_t *node)
{
    printf("PARAM v%d", node->var_no);
}

void print_intercode_node_read(intercode_node_read_t *node)
{
    printf("READ ");
    print_operand(node->var);
}

void print_intercode_node_write(intercode_node_write_t *node)
{
    printf("WRITE ");
    print_operand(node->var);
}

const char *relop_not(const char *relop)
{
    if (strcmp(relop, ">") == 0)
        return "<=";
    else if (strcmp(relop, ">=") == 0)
        return "<";
    else if (strcmp(relop, "<") == 0)
        return ">=";
    else if (strcmp(relop, "<=") == 0)
        return ">";
    else if (strcmp(relop, "==") == 0)
        return "!=";
    else if (strcmp(relop, "!=") == 0)
        return "==";
    assert(0);
    return "";    
}

intercode_line_t *create_intercode_line(intercode_node_t *node)
{
    intercode_line_t *rtn = malloc(sizeof(intercode_line_t));
    rtn->node = node;
    rtn->next = NULL;
    rtn->prev = NULL;
    return rtn;
}

void init_intercode_list()
{
    init_label_no();
    intercode_list.size = 0;
    intercode_list.start = NULL;
    intercode_list.end = NULL;
}

void intercode_list_push_back(intercode_node_t *node)
{
    assert(node != NULL);
    intercode_line_t *line = create_intercode_line(node);
    if (intercode_list.size == 0)
    {
        intercode_list.start = line;
    }
    else
    {
        line->prev = intercode_list.end;
        intercode_list.end->next = line;
    }
    intercode_list.end = line;
    intercode_list.size++;
    if (intercode_list.end->node->kind == CODE_ASSIGN)
    {
        intercode_list_check_assign();
    }
    
}

void intercode_list_check_assign()
{
    assert(intercode_list.end->node->kind == CODE_ASSIGN);
    intercode_node_assign_t *curr = (intercode_node_assign_t *)intercode_list.end->node;
    operand_t *t_val_1;
    operand_t *t_val_2 = ((intercode_node_assign_t *)intercode_list.end->node)->right;
    if (intercode_list.end->prev->node->kind == CODE_ASSIGN)
        t_val_1 = ((intercode_node_assign_t *)intercode_list.end->prev->node)->left;
    else if (intercode_list.end->prev->node->kind == CODE_OPERATOR)
        t_val_1 = ((intercode_node_binary_t *)intercode_list.end->prev->node)->target;
    else if (intercode_list.end->prev->node->kind == CODE_CALL)
        t_val_1 = ((intercode_node_call_t *)intercode_list.end->prev->node)->call_rtn;
    if (t_val_1->kind == OPERAND_VARIABLE_T && t_val_2->kind == OPERAND_VARIABLE_T && t_val_1->var_no == t_val_2->var_no)
    {
        if (intercode_list.end->prev->node->kind == CODE_ASSIGN)
        {
            intercode_node_assign_t *prev = (intercode_node_assign_t *)intercode_list.end->prev->node;
            prev->left = curr->left;
        }
        else if (intercode_list.end->prev->node->kind == CODE_OPERATOR)
        {
            intercode_node_binary_t *prev = (intercode_node_binary_t *)intercode_list.end->prev->node;
            prev->target = curr->left;
        }
        else if (intercode_list.end->prev->node->kind == CODE_CALL)
        {
            intercode_node_call_t *prev = (intercode_node_call_t *)intercode_list.end->prev->node;
            prev->call_rtn = curr->left;
        }
        intercode_list.end = intercode_list.end->prev;
        intercode_list.end->next = NULL;
        intercode_list.size = intercode_list.size - 1;
    }
}

void print_intercode_list()
{
    if (intercode_list.size == 0)
        return;
    for (intercode_line_t *itor = intercode_list.start; itor != NULL; itor = itor->next)
    {
        print_intercode_node(itor->node);
        printf("\n");
    }
}

intercode_line_t *get_intercode_list_start()
{
    return intercode_list.start;
}