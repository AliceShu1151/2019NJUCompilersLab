#include "intercode_type.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

operand_t *create_operand(int operand_kind, int val)
{
    operand_t *rtn = malloc(sizeof(operand_t));
    rtn->kind = operand_kind;
    if (operand_kind == OPERAND_ADDRESS)
        rtn->addr = val;
    else if (operand_kind == OPERAND_CONSTANT)
        rtn->value = val;
    else if (operand_kind == OPERAND_VARIABLE)
        rtn->var_no = val;
    return rtn;
}

void print_operand(operand_t *operand)
{
    assert(operand != NULL);
    if (operand->kind == OPERAND_VARIABLE)
        printf("t%d", operand->var_no);
    else if (operand->kind == OPERAND_CONSTANT)
        printf("#%d", operand->value);
    else if (operand->kind == OPERAND_ADDRESS)
        printf("%d", operand->addr);
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

static int var_no;
static int label_no;

void init_var_no()
{
    var_no = 1;
}

int malloc_var_no()
{
    return var_no++;
}

void init_label_no()
{
    label_no = 1;
}

int malloc_label_no()
{
    return label_no++;
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

intercode_node_t *create_intercode_node_if_goto(operand_t *if_left, operand_t *if_right, int target_label)
{
    intercode_node_if_goto_t *rtn = malloc(sizeof(intercode_node_if_goto_t));
    rtn->kind = CODE_IF_GOTO;
    rtn->if_left = if_left;
    rtn->if_right = if_right;
    rtn->target_label = target_label;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_return(operand_t *ret)
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

intercode_node_t *create_intercode_node_arg(int var_no)
{
    intercode_node_arg_t *rtn = malloc(sizeof(intercode_node_arg_t));
    rtn->kind = CODE_ARG;
    rtn->var_no = var_no;
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

intercode_node_t *create_intercode_node_read(int var_no)
{
    intercode_node_read_t *rtn = malloc(sizeof(intercode_node_read_t));
    rtn->kind = CODE_READ;
    rtn->var_no = var_no;
    return (intercode_node_t *)rtn;
}

intercode_node_t *create_intercode_node_write(int var_no)
{
    intercode_node_write_t *rtn = malloc(sizeof(intercode_node_write_t));
    rtn->kind = CODE_WRITE;
    rtn->var_no = var_no;
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
    print_operator(node->operator_kind);
    print_operand(node->if_right);
    printf("GOTO L%d", node->target_label);
}

void print_intercode_node_return(intercode_node_return_t *node)
{
    printf("RETURN ");
    print_operand(node->ret);
}

void print_intercode_node_dec(intercode_node_dec_t *node)
{
    printf("DEC t%d [%d]", node->var_no, node->size);
}

void print_intercode_node_arg(intercode_node_arg_t *node)
{
    printf("ARG t%d", node->var_no);
}

void print_intercode_node_call(intercode_node_call_t *node)
{
    print_operand(node->call_rtn);
    printf(" := CALL %s", node->call_name);
}

void print_intercode_node_param(intercode_node_param_t *node)
{
    printf("PARAM t%d", node->var_no);
}

void print_intercode_node_read(intercode_node_read_t *node)
{
    printf("READ t%d", node->var_no);
}

void print_intercode_node_write(intercode_node_write_t *node)
{
    printf("WRITE t%d", node->var_no);
}


void print_intercode_list(intercode_list_t *list)
{
    if (list->size == 0) 
        return;
    for(intercode_line_t *itor = list->start; itor != NULL; itor = itor->next)
    {
        print_intercode_node(itor->node);
        printf("\n");
    }
}