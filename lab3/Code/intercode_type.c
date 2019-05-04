#include "intercode_type.h"
#include <stdlib.h>

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
