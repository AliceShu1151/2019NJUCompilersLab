#include "MIPS32_translate.h"

static int arg_num = 0;
static int param_num = 0;
static int is_write_arg = 0;

void line_inter_to_MIPS_translate(intercode_node_t *node);
void line_inter_to_MIPS_translate_label(intercode_node_label_t *node);
void line_inter_to_MIPS_translate_func(intercode_node_func_t *node);
void line_inter_to_MIPS_translate_assign(intercode_node_assign_t *node);
void line_inter_to_MIPS_translate_binary(intercode_node_binary_t *node);
void line_inter_to_MIPS_translate_ref(intercode_node_ref_t *node);
void line_inter_to_MIPS_translate_dref(intercode_node_dref_t *node);
void line_inter_to_MIPS_translate_dref_assign(intercode_node_dref_assign_t *node);
void line_inter_to_MIPS_translate_goto(intercode_node_goto_t *node);
void line_inter_to_MIPS_translate_if_goto(intercode_node_if_goto_t *node);
void line_inter_to_MIPS_translate_return(intercode_node_return_t *node);
void line_inter_to_MIPS_translate_dec(intercode_node_dec_t *node);
void line_inter_to_MIPS_translate_arg(intercode_node_arg_t *node);
void line_inter_to_MIPS_translate_call(intercode_node_call_t *node);
void line_inter_to_MIPS_translate_param(intercode_node_param_t *node);
void line_inter_to_MIPS_translate_read(intercode_node_read_t *node);
void line_inter_to_MIPS_translate_write(intercode_node_write_t *node);

void MIPS32_translate()
{
    //print_intercode_list();
    init_MIPS_code_list();
    init_MIPS_reg_list();
    intercode_line_t *itor = get_intercode_list_start();
    while (itor != NULL)
    {
        if (itor->node->kind == CODE_ARG && itor->next->node->kind == CODE_WRITE)
            is_write_arg = 1;
        line_inter_to_MIPS_translate(itor->node);
        if (itor->node->kind == CODE_FUNC) 
        {
            create_var_table(itor->next);
            prologue();
            //print_var_table_top();
        }
        itor = itor->next;
    }
    print_MIPS_code_list();
}

void line_inter_to_MIPS_translate(intercode_node_t *node)
{
    init_reg_no();
    if (node->kind == CODE_LABEL)
        line_inter_to_MIPS_translate_label((intercode_node_label_t *)node);
    else if (node->kind == CODE_FUNC)
        line_inter_to_MIPS_translate_func((intercode_node_func_t *)node);
    else if (node->kind == CODE_ASSIGN)
        line_inter_to_MIPS_translate_assign((intercode_node_assign_t *)node);
    else if (node->kind == CODE_OPERATOR)
        line_inter_to_MIPS_translate_binary((intercode_node_binary_t *)node);
    else if (node->kind == CODE_REF)
        line_inter_to_MIPS_translate_ref((intercode_node_ref_t *)node);
    else if (node->kind == CODE_DREF)
        line_inter_to_MIPS_translate_dref((intercode_node_dref_t *)node);
    else if (node->kind == CODE_DREF_ASSIGN)
        line_inter_to_MIPS_translate_dref_assign((intercode_node_dref_assign_t *)node);
    else if (node->kind == CODE_GOTO)
        line_inter_to_MIPS_translate_goto((intercode_node_goto_t *)node);
    else if (node->kind == CODE_IF_GOTO)
        line_inter_to_MIPS_translate_if_goto((intercode_node_if_goto_t *)node);
    else if (node->kind == CODE_RETURN)
        line_inter_to_MIPS_translate_return((intercode_node_return_t *)node);
    else if (node->kind == CODE_DEC)
        line_inter_to_MIPS_translate_dec((intercode_node_dec_t *)node);
    else if (node->kind == CODE_ARG)
        line_inter_to_MIPS_translate_arg((intercode_node_arg_t *)node);
    else if (node->kind == CODE_CALL)
        line_inter_to_MIPS_translate_call((intercode_node_call_t *)node);
    else if (node->kind == CODE_PARAM)
        line_inter_to_MIPS_translate_param((intercode_node_param_t *)node);
    else if (node->kind == CODE_READ)
        line_inter_to_MIPS_translate_read((intercode_node_read_t *)node);
    else if (node->kind == CODE_WRITE)
        line_inter_to_MIPS_translate_write((intercode_node_write_t *)node);
}

void line_inter_to_MIPS_translate_label(intercode_node_label_t *node)
{
    MIPS_code_node_t *MIPS_node = create_MIPS_code_node_label(node->label);
    MIPS_code_list_push_back(MIPS_node);
}

void line_inter_to_MIPS_translate_func(intercode_node_func_t *node)
{
    assert(node->kind = CODE_FUNC);
    MIPS_code_node_t *MIPS_node = create_MIPS_code_node_func(node->func_name);
    MIPS_code_list_push_back(MIPS_node);
    param_num = 0;
}

void line_inter_to_MIPS_translate_assign(intercode_node_assign_t *node)
{
    MIPS_operand_t *left = create_MIPS_operand_const_or_reg(node->left);
    MIPS_operand_t *right;
    MIPS_code_node_t *MIPS_node;
    if (node->right->kind == OPERAND_CONSTANT_I)
    {
        right = create_MIPS_operand_const(node->right->value_int);
        MIPS_node = create_MIPS_code_node_li(left, right);
    }
    else
    {
        right = load_reg(node->right);
        MIPS_node = create_MIPS_code_node_move(left, right);
    }
    MIPS_code_list_push_back(MIPS_node);
    store_reg(left, node->left);
}

void line_inter_to_MIPS_translate_binary(intercode_node_binary_t *node)
{
    MIPS_operand_t *target = create_MIPS_operand_reg();
    MIPS_operand_t *left = load_reg(node->left);
    MIPS_operand_t *right;
    MIPS_code_node_t *MIPS_node;
    if (node->operator_kind == OPERATOR_PLS)
    {
        if (node->right->kind == OPERAND_CONSTANT_I || node->right->kind == OPERAND_CONSTANT_F)
        {
            right = create_MIPS_operand_const(node->right->value_int);
            MIPS_node = create_MIPS_code_node_addi(target, left, right);
        }
        else if (node->right->kind == OPERAND_VARIABLE_T || node->right->kind == OPERAND_VARIABLE_V)
        {
            right = load_reg(node->right);
            MIPS_node = create_MIPS_code_node_add(target, left, right);
        }
    }
    else if (node->operator_kind == OPERATOR_MIN)
    {
        if (node->right->kind == OPERAND_CONSTANT_I || node->right->kind == OPERAND_CONSTANT_F)
        {
            right = create_MIPS_operand_const(node->right->value_int*(-1));
            MIPS_node = create_MIPS_code_node_addi(target, left, right);
        }
        else if (node->right->kind == OPERAND_VARIABLE_T || node->right->kind == OPERAND_VARIABLE_V)
        {
            right = load_reg(node->right);
            MIPS_node = create_MIPS_code_node_sub(target, left, right);
        }
    }
    else
    {
        right = load_reg(node->right);
        if (node->operator_kind == OPERATOR_STAR)
            MIPS_node = create_MIPS_code_node_mul(target, left, right);
        else if (node->operator_kind == OPERATOR_DIV)
        {
            MIPS_code_node_t *MIPS_node_div = create_MIPS_code_node_div(left, right);
            MIPS_code_list_push_back(MIPS_node_div);
            MIPS_node = create_MIPS_code_node_mflo(target);
        }       
    }
    MIPS_code_list_push_back(MIPS_node);
    store_reg(target, node->target);
}

void line_inter_to_MIPS_translate_ref(intercode_node_ref_t *node)
{
    MIPS_operand_t *address = create_MIPS_operand_reg();
    MIPS_operand_t *fp_reg_operand = create_MIPS_operand_reg();
    fp_reg_operand->reg = create_reg_fp();
    MIPS_code_node_t *MIPS_node_addi = create_MIPS_code_node_addi(address, fp_reg_operand, create_MIPS_operand_const(get_MIPS_var_offset(node->right)));
    MIPS_code_list_push_back(MIPS_node_addi);
    store_reg(address, node->left);
}

void line_inter_to_MIPS_translate_dref(intercode_node_dref_t *node)
{
    MIPS_operand_t *left = create_MIPS_operand_reg();
    MIPS_operand_t *right = load_reg(node->right);
    MIPS_operand_t *right_address = create_MIPS_operand_reg_offset(right->reg, 0);
    MIPS_code_node_t *MIPS_node_lw = create_MIPS_code_node_lw(left, right_address);
    MIPS_code_list_push_back(MIPS_node_lw);
    store_reg(left, node->left);
}

void line_inter_to_MIPS_translate_dref_assign(intercode_node_dref_assign_t *node)
{
    MIPS_operand_t *left = load_reg(node->left);
    MIPS_operand_t *right = load_reg(node->right);
    MIPS_operand_t *left_address = create_MIPS_operand_reg_offset(left->reg, 0);
    MIPS_code_node_t *MIPS_node_sw = create_MIPS_code_node_sw(right, left_address);
    MIPS_code_list_push_back(MIPS_node_sw);
}

void line_inter_to_MIPS_translate_goto(intercode_node_goto_t *node)
{
    MIPS_code_node_t *MIPS_node_j = create_MIPS_code_node_j(create_MIPS_operand_label(node->label));
    MIPS_code_list_push_back(MIPS_node_j);
}

void line_inter_to_MIPS_translate_if_goto(intercode_node_if_goto_t *node)
{
    MIPS_operand_t *left = load_reg(node->if_left);
    MIPS_operand_t *right = load_reg(node->if_right);
    
    MIPS_code_node_t *MIPS_node;
    MIPS_operand_t *label = create_MIPS_operand_label(node->target_label);
    if (strcmp(node->relop, "==") == 0)
        MIPS_node = create_MIPS_code_node_b(MIPS_B_EQ, left, right, label);
    else if (strcmp(node->relop, "!=") == 0)
        MIPS_node = create_MIPS_code_node_b(MIPS_B_NE, left, right, label);
    else if (strcmp(node->relop, ">") == 0)
        MIPS_node = create_MIPS_code_node_b(MIPS_B_GT, left, right, label);
    else if (strcmp(node->relop, "<") == 0)
        MIPS_node = create_MIPS_code_node_b(MIPS_B_LT, left, right, label);
    else if (strcmp(node->relop, ">=") == 0)
        MIPS_node = create_MIPS_code_node_b(MIPS_B_GE, left, right, label);
    else if (strcmp(node->relop, "<=") == 0)
        MIPS_node = create_MIPS_code_node_b(MIPS_B_LE, left, right, label);
    MIPS_code_list_push_back(MIPS_node);
}

void line_inter_to_MIPS_translate_return(intercode_node_return_t *node)
{
    MIPS_operand_t *ret = load_reg(node->ret);
    MIPS_code_node_t *MIPS_node_rtn = create_MIPS_code_node_move(create_MIPS_operand_string("$v0"),ret);
    MIPS_code_list_push_back(MIPS_node_rtn);

    epilogue();
    MIPS_code_node_t *MIPS_node_jr = create_MIPS_code_node_jr(create_MIPS_operand_string("$ra"));
    MIPS_code_list_push_back(MIPS_node_jr);
}

void line_inter_to_MIPS_translate_dec(intercode_node_dec_t *node)
{
    return;
}

void line_inter_to_MIPS_translate_arg(intercode_node_arg_t *node)
{
    if (is_write_arg)
    {
        MIPS_operand_t *arg = load_reg(node->arg);
        MIPS_code_node_t *MIPS_node_move = create_MIPS_code_node_move(create_MIPS_operand_string("$a0"), arg);
        MIPS_code_list_push_back(MIPS_node_move);
    }
    else
    {
        MIPS_operand_t *arg = load_reg(node->arg);
        MIPS_code_node_t *MIPS_node_sw = create_MIPS_code_node_sw(arg, create_MIPS_operand_reg_offset(create_reg_sp(), (-4)*arg_num));
        MIPS_code_list_push_back(MIPS_node_sw);
    }
    arg_num++;
}

void line_inter_to_MIPS_translate_call(intercode_node_call_t *node)
{
    char *s_1 = malloc(sizeof(char)*100);
    char *s_2 = malloc(sizeof(char)*100);
    sprintf(s_1, "\tsubu $sp, $sp, %d", (4)*(arg_num-1));
    sprintf(s_2, "\taddi $sp, $sp, %d", (4)*(arg_num-1));

    MIPS_code_node_t *nd_1 = create_MIPS_code_node_string(s_1);
    MIPS_code_list_push_back(nd_1);

    MIPS_code_node_t *nd_2 = create_MIPS_code_node_jal(create_MIPS_operand_string(node->call_name));
    MIPS_code_list_push_back(nd_2);

    MIPS_code_node_t *nd_3 = create_MIPS_code_node_string(s_2);
    MIPS_code_list_push_back(nd_3);

    MIPS_operand_t *ret = create_MIPS_operand_reg();
    MIPS_code_node_t *nd_4 = create_MIPS_code_node_move(ret,create_MIPS_operand_string("$v0"));
    MIPS_code_list_push_back(nd_4);
    store_reg(ret, node->call_rtn);

    arg_num = 0;
}

void line_inter_to_MIPS_translate_param(intercode_node_param_t *node)
{
    MIPS_reg_t *fp = create_reg_fp();
    int dst_offset = get_MIPS_var_offset(create_operand_var(OPERAND_VARIABLE_V, node->var_no));
    MIPS_operand_t *dst = create_MIPS_operand_reg_offset(fp, dst_offset);
    MIPS_operand_t *src = create_MIPS_operand_reg_offset(fp, param_num*(-4));

    MIPS_operand_t *src_reg = create_MIPS_operand_reg();
    MIPS_code_node_t *lw = create_MIPS_code_node_lw(src_reg,src);
    MIPS_code_list_push_back(lw);

    MIPS_code_node_t *sw = create_MIPS_code_node_sw(src_reg,dst);
    MIPS_code_list_push_back(sw);
    param_num++;
}

void line_inter_to_MIPS_translate_read(intercode_node_read_t *node)
{
    MIPS_code_node_t *s_1 = create_MIPS_code_node_string("\taddi $sp, $sp, -4");
    MIPS_code_node_t *s_2 = create_MIPS_code_node_string("\tsw $ra, 0($sp)");
    MIPS_code_node_t *s_3 = create_MIPS_code_node_string("\tjal read");
    MIPS_code_node_t *s_4 = create_MIPS_code_node_string("\tlw $ra, 0($sp)");
    MIPS_code_node_t *s_5 = create_MIPS_code_node_string("\taddi $sp, $sp, 4");
    MIPS_code_list_push_back(s_1);
    MIPS_code_list_push_back(s_2);
    MIPS_code_list_push_back(s_3);
    MIPS_code_list_push_back(s_4);
    MIPS_code_list_push_back(s_5);
    MIPS_operand_t *MIPS_reg = create_MIPS_operand_reg();
    MIPS_code_node_t *s = create_MIPS_code_node_move(MIPS_reg, create_MIPS_operand_string("$v0"));
    MIPS_code_list_push_back(s);
    store_reg(MIPS_reg, node->var);
}

void line_inter_to_MIPS_translate_write(intercode_node_write_t *node)
{
    MIPS_code_node_t *s_1 = create_MIPS_code_node_string("\taddi $sp, $sp, -4");
    MIPS_code_node_t *s_2 = create_MIPS_code_node_string("\tsw $ra, 0($sp)");
    MIPS_code_node_t *s_3 = create_MIPS_code_node_string("\tjal write");
    MIPS_code_node_t *s_4 = create_MIPS_code_node_string("\tlw $ra, 0($sp)");
    MIPS_code_node_t *s_5 = create_MIPS_code_node_string("\taddi $sp, $sp, 4");
    MIPS_code_list_push_back(s_1);
    MIPS_code_list_push_back(s_2);
    MIPS_code_list_push_back(s_3);
    MIPS_code_list_push_back(s_4);
    MIPS_code_list_push_back(s_5);
    is_write_arg = 0;
    arg_num = 0;
}