#include "MIPS32_type.h"

static MIPS_code_list_t MIPS_code_list;

MIPS_operand_t *create_MIPS_operand_const(int ival)
{
    MIPS_operand_t *rtn = malloc(sizeof(MIPS_operand_t));
    rtn->kind = MIPS_OPERAND_CONST;
    rtn->ival = ival;
    return rtn;
}

MIPS_operand_t *create_MIPS_operand_label(int label)
{
    MIPS_operand_t *rtn = malloc(sizeof(MIPS_operand_t));
    rtn->kind = MIPS_OPERAND_LABEL;
    rtn->label = label;
    return rtn;
}

MIPS_operand_t *create_MIPS_operand_reg(const char *reg_type, int reg_no)
{
    MIPS_operand_t *rtn = malloc(sizeof(MIPS_operand_t));
    rtn->kind = MIPS_OPERAND_REG;
    rtn->reg_type = reg_type;
    rtn->reg_no = reg_no;
    return rtn;
}

MIPS_operand_t *create_MIPS_operand_offset(const char *reg_type, int reg_no, int reg_offset)
{
    MIPS_operand_t *rtn = malloc(sizeof(MIPS_operand_t));
    rtn->kind = MIPS_OPERAND_REG_OFFSET;
    rtn->reg_type = reg_type;
    rtn->reg_no = reg_no;
    rtn->reg_offset = reg_offset;
    return rtn;
}

MIPS_operand_t *create_MIPS_operand_string(const char *content)
{
    MIPS_operand_t *rtn = malloc(sizeof(MIPS_operand_t));
    rtn->kind = MIPS_OPERAND_STRING;
    rtn->content = content;
    return rtn;
}

void print_MIPS_operand(MIPS_operand_t *MIPS_operand)
{
    if (MIPS_operand->kind == MIPS_OPERAND_CONST)
    {
        printf("%d", MIPS_operand->ival);
    }
    else if (MIPS_operand->kind == MIPS_OPERAND_LABEL)
    {
        printf("label%d", MIPS_operand->label);
    }
    else if (MIPS_operand->kind == MIPS_OPERAND_REG)
    {
        printf("$%s%d", MIPS_operand->reg_type, MIPS_operand->reg_no);
    }
    else if (MIPS_operand->kind == MIPS_OPERAND_REG_OFFSET)
    {
        printf("%d($%s%d)", MIPS_operand->reg_offset, MIPS_operand->reg_type, MIPS_operand->reg_no);
    }
    else if (MIPS_operand->kind == MIPS_OPERAND_STRING)
    {
        printf("%s", MIPS_operand->content);
    }
}

MIPS_code_node_t *create_MIPS_code_node_label(int label)
{
    MIPS_code_node_label_t *rtn = malloc(sizeof(MIPS_code_node_label_t));
    rtn->kind = MIPS_LABEL;
    rtn->label = label;
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_li(MIPS_operand_t *dst, MIPS_operand_t *src)
{
    MIPS_code_node_li_t *rtn = malloc(sizeof(MIPS_code_node_li_t));
    rtn->kind = MIPS_LI;
    rtn->dst = dst;
    rtn->src = src;
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_la(MIPS_operand_t *dst, MIPS_operand_t *src)
{
    MIPS_code_node_la_t *rtn = malloc(sizeof(MIPS_code_node_la_t));
    rtn->kind = MIPS_LA;
    rtn->dst = dst;
    rtn->src = src;
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_move(MIPS_operand_t *dst, MIPS_operand_t *src)
{
    MIPS_code_node_move_t *rtn = malloc(sizeof(MIPS_code_node_move_t));
    rtn->kind = MIPS_MOVE;
    rtn->dst = dst;
    rtn->src = src;
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_addi(MIPS_operand_t *dst, MIPS_operand_t *src_l, MIPS_operand_t *src_r)
{
    MIPS_code_node_addi_t *rtn = malloc(sizeof(MIPS_code_node_addi_t));
    rtn->kind = MIPS_ADDI;
    rtn->dst = dst;
    rtn->src_l = src_l;
    rtn->src_r = src_r;
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_add(MIPS_operand_t *dst, MIPS_operand_t *src_l, MIPS_operand_t *src_r)
{
    MIPS_code_node_add_t *rtn = malloc(sizeof(MIPS_code_node_add_t));
    rtn->kind = MIPS_ADD;
    rtn->dst = dst;
    rtn->src_l = src_l;
    rtn->src_r = src_r;
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_sub(MIPS_operand_t *dst, MIPS_operand_t *src_l, MIPS_operand_t *src_r)
{
    MIPS_code_node_sub_t *rtn = malloc(sizeof(MIPS_code_node_sub_t));
    rtn->kind = MIPS_SUB;
    rtn->dst = dst;
    rtn->src_l = src_l;
    rtn->src_r = src_r;
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_mul(MIPS_operand_t *dst, MIPS_operand_t *src_l, MIPS_operand_t *src_r)
{
    MIPS_code_node_mul_t *rtn = malloc(sizeof(MIPS_code_node_mul_t));
    rtn->kind = MIPS_MUL;
    rtn->dst = dst;
    rtn->src_l = src_l;
    rtn->src_r = src_r;
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_div(MIPS_operand_t *dst, MIPS_operand_t *src)
{
    MIPS_code_node_div_t *rtn = malloc(sizeof(MIPS_code_node_div_t));
    rtn->kind = MIPS_DIV;
    rtn->dst = dst;
    rtn->src = src;
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_mflo(MIPS_operand_t *reg)
{
    MIPS_code_node_mflo_t *rtn = malloc(sizeof(MIPS_code_node_mflo_t));
    rtn->kind = MIPS_MFLO;
    rtn->reg = reg;
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_lw(MIPS_operand_t *dst, MIPS_operand_t *src)
{
    MIPS_code_node_lw_t *rtn = malloc(sizeof(MIPS_code_node_lw_t));
    rtn->kind = MIPS_LW;
    rtn->dst = dst;
    rtn->src = src;
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_sw(MIPS_operand_t *dst, MIPS_operand_t *src)
{
    MIPS_code_node_sw_t *rtn = malloc(sizeof(MIPS_code_node_sw_t));
    rtn->kind = MIPS_DIV;
    rtn->dst = dst;
    rtn->src = src;
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_j(MIPS_operand_t *label)
{
    MIPS_code_node_j_t *rtn = malloc(sizeof(MIPS_code_node_j_t));
    rtn->kind = MIPS_J;
    rtn->label = label;
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_jar(MIPS_operand_t *func)
{
    MIPS_code_node_jar_t *rtn = malloc(sizeof(MIPS_code_node_jar_t));
    rtn->kind = MIPS_JAR;
    rtn->func = func;
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_jr(MIPS_operand_t *reg)
{
    MIPS_code_node_jr_t *rtn = malloc(sizeof(MIPS_code_node_jr_t));
    rtn->kind = MIPS_JR;
    rtn->reg = reg;
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_b(int MIPS_B_kind, MIPS_operand_t *left, MIPS_operand_t *right, MIPS_operand_t *label)
{
    MIPS_code_node_b_t *rtn = malloc(sizeof(MIPS_code_node_b_t));
    rtn->kind = MIPS_B;
    rtn->MIPS_B_kind = MIPS_B_kind;
    rtn->left = left;
    rtn->right = right;
    rtn->label = label;
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_call()
{
    MIPS_code_node_call_t *rtn = malloc(sizeof(MIPS_code_node_call_t));
    rtn->kind = MIPS_CALL;
    rtn->syscall = "systcall";
    return (MIPS_code_node_t*)rtn;
}

MIPS_code_node_t *create_MIPS_code_node_func(const char *func)
{
    MIPS_code_node_func_t *rtn = malloc(sizeof(MIPS_code_node_func_t));
    rtn->kind = MIPS_FUNC;
    rtn->func = func;
    return (MIPS_code_node_t*)rtn;
}
