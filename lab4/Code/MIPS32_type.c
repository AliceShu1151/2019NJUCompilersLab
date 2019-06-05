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

MIPS_code_node_t *create_MIPS_code_node_jal(MIPS_operand_t *func)
{
    MIPS_code_node_jal_t *rtn = malloc(sizeof(MIPS_code_node_jal_t));
    rtn->kind = MIPS_JAL;
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

MIPS_code_node_t *create_MIPS_code_node_string(const char *content)
{
    MIPS_code_node_string_t *rtn = malloc(sizeof(MIPS_code_node_string_t));
    rtn->kind = MIPS_STRING;
    rtn->content = content;
    return (MIPS_code_node_t*)rtn;
}

void print_MIPS_node(MIPS_code_node_t *node)
{
    if (node->kind == MIPS_LABEL)
        print_MIPS_node_label((MIPS_code_node_label_t *)node);
    else if (node->kind == MIPS_LI)
        print_MIPS_node_li((MIPS_code_node_li_t *)node);
    else if (node->kind == MIPS_LA)
        print_MIPS_node_la((MIPS_code_node_la_t *)node);
    else if (node->kind == MIPS_MOVE)
        print_MIPS_node_move((MIPS_code_node_move_t *)node);
    else if (node->kind == MIPS_ADDI)
        print_MIPS_node_addi((MIPS_code_node_addi_t *)node);
    else if (node->kind == MIPS_ADD)
        print_MIPS_node_add((MIPS_code_node_add_t *)node);
    else if (node->kind == MIPS_SUB)
        print_MIPS_node_sub((MIPS_code_node_sub_t *)node);
    else if (node->kind == MIPS_MUL)
        print_MIPS_node_mul((MIPS_code_node_mul_t *)node);
    else if (node->kind == MIPS_DIV)
        print_MIPS_node_div((MIPS_code_node_div_t *)node);
    else if (node->kind == MIPS_MFLO)
        print_MIPS_node_mflo((MIPS_code_node_mflo_t *)node);
    else if (node->kind == MIPS_LW)
        print_MIPS_node_lw((MIPS_code_node_lw_t *)node);
    else if (node->kind == MIPS_SW)
        print_MIPS_node_sw((MIPS_code_node_sw_t *)node);
    else if (node->kind == MIPS_J)
        print_MIPS_node_j((MIPS_code_node_j_t *)node);
    else if (node->kind == MIPS_JAL)
        print_MIPS_node_jal((MIPS_code_node_jal_t *)node);
    else if (node->kind == MIPS_JR)
        print_MIPS_node_jr((MIPS_code_node_jr_t *)node);
    else if (node->kind == MIPS_J)
        print_MIPS_node_b((MIPS_code_node_b_t *)node);
    else if (node->kind == MIPS_CALL)
        print_MIPS_node_call((MIPS_code_node_call_t *)node);
    else if (node->kind == MIPS_FUNC)
        print_MIPS_node_func((MIPS_code_node_func_t *)node);
    else if (node->kind == MIPS_STRING)
        print_MIPS_node_string((MIPS_code_node_string_t *)node);
}

void print_MIPS_node_label(MIPS_code_node_label_t *node)
{
    printf("label%d:", node->label);
}


void print_MIPS_node_li(MIPS_code_node_li_t *node)
{
    printf("\tli ");
    print_MIPS_operand(node->dst);
    printf(", ");
    print_MIPS_operand(node->src);
}

void print_MIPS_node_la(MIPS_code_node_la_t *node)
{
    printf("\tla ");
    print_MIPS_operand(node->dst);
    printf(", ");
    print_MIPS_operand(node->src);
}

void print_MIPS_node_move(MIPS_code_node_move_t *node)
{
    printf("\tmove ");
    print_MIPS_operand(node->dst);
    printf(", ");
    print_MIPS_operand(node->src);
}

void print_MIPS_node_addi(MIPS_code_node_addi_t *node)
{
    printf("\taddi ");
    print_MIPS_operand(node->dst);
    printf(", ");
    print_MIPS_operand(node->src_l);
    printf(", ");
    print_MIPS_operand(node->src_r);
}

void print_MIPS_node_add(MIPS_code_node_add_t *node)
{
    printf("\tadd ");
    print_MIPS_operand(node->dst);
    printf(", ");
    print_MIPS_operand(node->src_l);
    printf(", ");
    print_MIPS_operand(node->src_r);
}

void print_MIPS_node_sub(MIPS_code_node_sub_t *node)
{
    printf("\tsub ");
    print_MIPS_operand(node->dst);
    printf(", ");
    print_MIPS_operand(node->src_l);
    printf(", ");
    print_MIPS_operand(node->src_r);
}

void print_MIPS_node_mul(MIPS_code_node_mul_t *node)
{
    printf("\tmul ");
    print_MIPS_operand(node->dst);
    printf(", ");
    print_MIPS_operand(node->src_l);
    printf(", ");
    print_MIPS_operand(node->src_r);
}

void print_MIPS_node_div(MIPS_code_node_div_t *node)
{
    printf("\tdiv ");
    print_MIPS_operand(node->dst);
    printf(", ");
    print_MIPS_operand(node->src);
}

void print_MIPS_node_mflo(MIPS_code_node_mflo_t *node)
{
    printf("\tmflo ");
    print_MIPS_operand(node->reg);
}

void print_MIPS_node_lw(MIPS_code_node_lw_t *node)
{
    printf("\tlw ");
    print_MIPS_operand(node->dst);
    printf(", ");
    print_MIPS_operand(node->src);
}

void print_MIPS_node_sw(MIPS_code_node_sw_t *node)
{
    printf("\tdiv ");
    print_MIPS_operand(node->dst);
    printf(", ");
    print_MIPS_operand(node->src);
}

void print_MIPS_node_j(MIPS_code_node_j_t *node)
{
    printf("\tj ");
    print_MIPS_operand(node->label);
}

void print_MIPS_node_jal(MIPS_code_node_jal_t *node)
{
    printf("\tjal ");
    print_MIPS_operand(node->func);
}

void print_MIPS_node_jr(MIPS_code_node_jr_t *node)
{
    printf("\tjr ");
    print_MIPS_operand(node->reg);
}

void print_MIPS_node_b(MIPS_code_node_b_t *node)
{
    if (node->MIPS_B_kind == MIPS_B_EQ)
        printf("\tbeq ");
    else if (node->MIPS_B_kind == MIPS_B_NE)
        printf("\tbne ");
    else if (node->MIPS_B_kind == MIPS_B_GT)
        printf("\tbgt ");
    else if (node->MIPS_B_kind == MIPS_B_LT)
        printf("\tblt ");
    else if (node->MIPS_B_kind == MIPS_B_GE)
        printf("\tbge ");
    else if (node->MIPS_B_kind == MIPS_B_LE)
        printf("\tble ");
    print_MIPS_operand(node->left);
    printf(", ");
    print_MIPS_operand(node->right);
    printf(", ");
    print_MIPS_operand(node->label);
}

void print_MIPS_node_call(MIPS_code_node_call_t *node)
{
    printf("\t%s", node->syscall);
}

void print_MIPS_node_func(MIPS_code_node_func_t *node)
{
    printf("%s:", node->func);
}

void print_MIPS_node_string(MIPS_code_node_string_t *node)
{
    printf("%s", node->content);
}


MIPS_code_line_t *create_MIPS_code_line(MIPS_code_node_t *node)
{
    MIPS_code_line_t *rtn = malloc(sizeof(MIPS_code_line_t));
    rtn->next = NULL;
    rtn->prev = NULL;
    rtn->node = node;
    return rtn;
}

void init_MIPS_code_list()
{
    MIPS_code_list.size = 0;
    MIPS_code_node_t *line[20];
    line[0] = create_MIPS_code_node_string(".data");
    line[1] = create_MIPS_code_node_string("_prompt: .asciiz \"Enter an interger:\"");
    line[2] = create_MIPS_code_node_string("_ret: .asciiz \"\\n\"");
    line[3] = create_MIPS_code_node_string(".text");
    line[4] = create_MIPS_code_node_string("read:");
    line[5] = create_MIPS_code_node_string("\tli $v0, 4");
    line[6] = create_MIPS_code_node_string("\tla $a0, _prompt");
    line[7] = create_MIPS_code_node_string("\tsyscall");
    line[8] = create_MIPS_code_node_string("\tli $v0, 5");
    line[9] = create_MIPS_code_node_string("\tsyscall");
    line[10] = create_MIPS_code_node_string("\tjr $ra\n");
    line[11] = create_MIPS_code_node_string("write:");
    line[12] = create_MIPS_code_node_string("\tli $v0, 1");
    line[13] = create_MIPS_code_node_string("\tsyscall");
    line[14] = create_MIPS_code_node_string("\tli $v0, 4");
    line[15] = create_MIPS_code_node_string("\tla $a0, _ret");
    line[16] = create_MIPS_code_node_string("\tsyscall");
    line[17] = create_MIPS_code_node_string("\tmove $v0, $0");
    line[18] = create_MIPS_code_node_string("\tjr $ra\n");
    for (size_t i = 0; i < 19; i++)
        MIPS_code_list_push_back(line[i]);
}   

void MIPS_code_list_push_back(MIPS_code_node_t *node)
{
    MIPS_code_line_t *line = create_MIPS_code_line(node);
    if (MIPS_code_list.size == 0)
    {
        MIPS_code_list.start = line;
    }
    else
    {
        line->prev = MIPS_code_list.end;
        MIPS_code_list.end->next = line;
    }
    MIPS_code_list.end = line;
    MIPS_code_list.size++;
}


void print_MIPS_code_list()
{
    if (MIPS_code_list.size == 0)
        return;
    for (MIPS_code_line_t *itor = MIPS_code_list.start; itor != NULL; itor = itor->next)
    {
        print_MIPS_node(itor->node);
        printf("\n");
    }
}