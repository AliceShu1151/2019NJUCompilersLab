#include "intercode_translate.h"

enum
{
    MIPS_OPERAND_CONST,
    MIPS_OPERAND_LABEL,
    MIPS_OPERAND_REG,
    MIPS_OPERAND_REG_OFFSET,
    MIPS_OPERAND_STRING
};

typedef struct MIPS_operand MIPS_operand_t;
struct MIPS_operand
{
    int kind;
    union {
        int ival;
        int label;
        const char *reg_type;
        const char *content;
    };
    int reg_no;
    int reg_offset;
};

MIPS_operand_t *create_MIPS_operand_const(int ival);
MIPS_operand_t *create_MIPS_operand_label(int label);
MIPS_operand_t *create_MIPS_operand_reg(const char *reg_type, int reg_no);
MIPS_operand_t *create_MIPS_operand_reg_offset(const char *reg_type, int reg_no, int reg_offset);
MIPS_operand_t *create_MIPS_operand_string(const char *content);
void print_MIPS_operand(MIPS_operand_t *operand);

enum
{
    MIPS_LABEL,
    MIPS_LI,
    MIPS_LA,
    MIPS_MOVE,
    MIPS_ADDI,
    MIPS_ADD,
    MIPS_SUB,
    MIPS_MUL,
    MIPS_DIV,
    MIPS_MFLO,
    MIPS_LW,
    MIPS_SW,
    MIPS_J,
    MIPS_JAL,
    MIPS_JR,
    MIPS_B,
    MIPS_CALL,
    MIPS_FUNC,
    MIPS_STRING
};
enum
{
    MIPS_B_EQ,
    MIPS_B_NE,
    MIPS_B_GT,
    MIPS_B_LT,
    MIPS_B_GE,
    MIPS_B_LE
};

typedef struct MIPS_code_node
{
    int kind;
} MIPS_code_node_t;

typedef struct MIPS_code_node_label
{
    int kind;
    int label;
} MIPS_code_node_label_t;

typedef struct MIPS_code_node_li
{
    int kind;
    MIPS_operand_t *dst;
    MIPS_operand_t *src;
} MIPS_code_node_li_t;

typedef struct MIPS_code_node_la
{
    int kind;
    MIPS_operand_t *dst;
    MIPS_operand_t *src;
} MIPS_code_node_la_t;

typedef struct MIPS_code_node_move
{
    int kind;
    MIPS_operand_t *dst;
    MIPS_operand_t *src;
} MIPS_code_node_move_t;

typedef struct MIPS_code_node_addi
{
    int kind;
    MIPS_operand_t *dst;
    MIPS_operand_t *src_l;
    MIPS_operand_t *src_r;
} MIPS_code_node_addi_t;

typedef struct MIPS_code_node_add
{
    int kind;
    MIPS_operand_t *dst;
    MIPS_operand_t *src_l;
    MIPS_operand_t *src_r;
} MIPS_code_node_add_t;

typedef struct MIPS_code_node_sub
{
    int kind;
    MIPS_operand_t *dst;
    MIPS_operand_t *src_l;
    MIPS_operand_t *src_r;
} MIPS_code_node_sub_t;

typedef struct MIPS_code_node_mul
{
    int kind;
    MIPS_operand_t *dst;
    MIPS_operand_t *src_l;
    MIPS_operand_t *src_r;
} MIPS_code_node_mul_t;

typedef struct MIPS_code_node_div
{
    int kind;
    MIPS_operand_t *dst;
    MIPS_operand_t *src;
} MIPS_code_node_div_t;

typedef struct MIPS_code_node_mflo
{
    int kind;
    MIPS_operand_t *reg;
} MIPS_code_node_mflo_t;

typedef struct MIPS_code_node_lw
{
    int kind;
    MIPS_operand_t *dst;
    MIPS_operand_t *src;
} MIPS_code_node_lw_t;

typedef struct MIPS_code_node_sw
{
    int kind;
    MIPS_operand_t *dst;
    MIPS_operand_t *src;
} MIPS_code_node_sw_t;

typedef struct MIPS_code_node_j
{
    int kind;
    MIPS_operand_t *label;
} MIPS_code_node_j_t;

typedef struct MIPS_code_node_jal
{
    int kind;
    MIPS_operand_t *func;
} MIPS_code_node_jal_t;

typedef struct MIPS_code_node_jr
{
    int kind;
    MIPS_operand_t *reg;
} MIPS_code_node_jr_t;

typedef struct MIPS_code_node_b
{
    int kind;
    int MIPS_B_kind;
    MIPS_operand_t *left;
    MIPS_operand_t *right;
    MIPS_operand_t *label;
} MIPS_code_node_b_t;

typedef struct MIPS_code_node_call
{
    int kind;
    const char *syscall;
} MIPS_code_node_call_t;

typedef struct MIPS_code_node_func
{
    int kind;
    const char *func;
} MIPS_code_node_func_t;

typedef struct MIPS_code_node_string
{
    int kind;
    const char *content;
} MIPS_code_node_string_t;

MIPS_code_node_t *create_MIPS_code_node_label(int label);
MIPS_code_node_t *create_MIPS_code_node_li(MIPS_operand_t *dst, MIPS_operand_t *src);
MIPS_code_node_t *create_MIPS_code_node_la(MIPS_operand_t *dst, MIPS_operand_t *src);
MIPS_code_node_t *create_MIPS_code_node_move(MIPS_operand_t *dst, MIPS_operand_t *src);
MIPS_code_node_t *create_MIPS_code_node_addi(MIPS_operand_t *dst, MIPS_operand_t *src_l, MIPS_operand_t *src_r);
MIPS_code_node_t *create_MIPS_code_node_add(MIPS_operand_t *dst, MIPS_operand_t *src_l, MIPS_operand_t *src_r);
MIPS_code_node_t *create_MIPS_code_node_sub(MIPS_operand_t *dst, MIPS_operand_t *src_l, MIPS_operand_t *src_r);
MIPS_code_node_t *create_MIPS_code_node_mul(MIPS_operand_t *dst, MIPS_operand_t *src_l, MIPS_operand_t *src_r);
MIPS_code_node_t *create_MIPS_code_node_div(MIPS_operand_t *dst, MIPS_operand_t *src);
MIPS_code_node_t *create_MIPS_code_node_mflo(MIPS_operand_t *reg);
MIPS_code_node_t *create_MIPS_code_node_lw(MIPS_operand_t *dst, MIPS_operand_t *src);
MIPS_code_node_t *create_MIPS_code_node_sw(MIPS_operand_t *dst, MIPS_operand_t *src);
MIPS_code_node_t *create_MIPS_code_node_j(MIPS_operand_t *label);
MIPS_code_node_t *create_MIPS_code_node_jal(MIPS_operand_t *func);
MIPS_code_node_t *create_MIPS_code_node_jr(MIPS_operand_t *reg);
MIPS_code_node_t *create_MIPS_code_node_b(int MIPS_B_kind, MIPS_operand_t *left, MIPS_operand_t *right, MIPS_operand_t *label);
MIPS_code_node_t *create_MIPS_code_node_call();
MIPS_code_node_t *create_MIPS_code_node_func(const char *func);
MIPS_code_node_t *create_MIPS_code_node_string(const char *content);

void print_MIPS_node(MIPS_code_node_t *node);
void print_MIPS_node_label(MIPS_code_node_label_t *node);
void print_MIPS_node_li(MIPS_code_node_li_t *node);
void print_MIPS_node_la(MIPS_code_node_la_t *node);
void print_MIPS_node_move(MIPS_code_node_move_t *node);
void print_MIPS_node_addi(MIPS_code_node_addi_t *node);
void print_MIPS_node_add(MIPS_code_node_add_t *node);
void print_MIPS_node_sub(MIPS_code_node_sub_t *node);
void print_MIPS_node_mul(MIPS_code_node_mul_t *node);
void print_MIPS_node_div(MIPS_code_node_div_t *node);
void print_MIPS_node_mflo(MIPS_code_node_mflo_t *node);
void print_MIPS_node_lw(MIPS_code_node_lw_t *node);
void print_MIPS_node_sw(MIPS_code_node_sw_t *node);
void print_MIPS_node_j(MIPS_code_node_j_t *node);
void print_MIPS_node_jal(MIPS_code_node_jal_t *node);
void print_MIPS_node_jr(MIPS_code_node_jr_t *node);
void print_MIPS_node_b(MIPS_code_node_b_t *node);
void print_MIPS_node_call(MIPS_code_node_call_t *node);
void print_MIPS_node_func(MIPS_code_node_func_t *node);
void print_MIPS_node_string(MIPS_code_node_string_t *node);

typedef struct MIPS_code_line MIPS_code_line_t;
struct MIPS_code_line
{
    MIPS_code_node_t *node;
    MIPS_code_line_t *prev;
    MIPS_code_line_t *next;
};

MIPS_code_line_t *create_MIPS_code_line(MIPS_code_node_t *node);

typedef struct MIPS_code_list MIPS_code_list_t;
struct MIPS_code_list
{
    int size;
    MIPS_code_line_t *start;
    MIPS_code_line_t *end;
};

void init_MIPS_code_list();
void MIPS_code_list_push_back(MIPS_code_node_t *node);

void print_MIPS_code_list();