#include "intercode_translate.h"

enum
{
    MIPS_OPERAND_CONST,
    MIPS_OPERAND_LABEL,
    MIPS_OPERAND_REG,
    MIPS_OPERAND_REG_OFFSET,
    MIPS_OPERAND_STRING
};

typedef struct MIPS_operand_reg MIPS_operand_reg_t;
struct MIPS_operand_reg
{
    int kind;
    union {
        int const_num;
        int label;
        const char *reg_type;
        const char *content;
    };
    int reg_no;
    int reg_offset;
};

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
    MIPS_B
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
    MIPS_operand_reg_t *dst;
    MIPS_operand_reg_t *src;
} MIPS_code_node_li_t;

typedef struct MIPS_code_node_la
{
    int kind;
    MIPS_operand_reg_t *dst;
    MIPS_operand_reg_t *src;
} MIPS_code_node_la_t;

typedef struct MIPS_code_node_move
{
    int kind;
    MIPS_operand_reg_t *dst;
    MIPS_operand_reg_t *src;
} MIPS_code_node_move_t;

typedef struct MIPS_code_node_addi
{
    int kind;
    MIPS_operand_reg_t *dst;
    MIPS_operand_reg_t *src_l;
    MIPS_operand_reg_t *src_r;
} MIPS_code_node_addi_t;

typedef struct MIPS_code_node_add
{
    int kind;
    MIPS_operand_reg_t *dst;
    MIPS_operand_reg_t *src_l;
    MIPS_operand_reg_t *src_r;
} MIPS_code_node_add_t;

typedef struct MIPS_code_node_sub
{
    int kind;
    MIPS_operand_reg_t *dst;
    MIPS_operand_reg_t *src_l;
    MIPS_operand_reg_t *src_r;
} MIPS_code_node_sub_t;

typedef struct MIPS_code_node_mul
{
    int kind;
    MIPS_operand_reg_t *dst;
    MIPS_operand_reg_t *src_l;
    MIPS_operand_reg_t *src_r;
} MIPS_code_node_mul_t;

typedef struct MIPS_code_node_div
{
    int kind;
    MIPS_operand_reg_t *dst;
    MIPS_operand_reg_t *src;
} MIPS_code_node_div_t;

typedef struct MIPS_code_node_mflo
{
    int kind;
    MIPS_operand_reg_t *dst;
    MIPS_operand_reg_t *src;
} MIPS_code_node_mflo_t;

typedef struct MIPS_code_node_lw
{
    int kind;
    MIPS_operand_reg_t *dst;
    MIPS_operand_reg_t *src;
} MIPS_code_node_lw_t;

typedef struct MIPS_code_node_sw
{
    int kind;
    MIPS_operand_reg_t *dst;
    MIPS_operand_reg_t *src;
} MIPS_code_node_sw_t;

typedef struct MIPS_code_node_j
{
    int kind;
    MIPS_operand_reg_t *label;
} MIPS_code_node_j_t;

typedef struct MIPS_code_node_jar
{
    int kind;
    const char *func;
} MIPS_code_node_jar_t;

typedef struct MIPS_code_node_jr
{
    int kind;
    MIPS_operand_reg_t *reg;
} MIPS_code_node_jr_t;

typedef struct MIPS_code_node_b
{
    int kind;
    int MIPS_B_kind;
    MIPS_operand_reg_t *left;
    MIPS_operand_reg_t *right;
    MIPS_operand_reg_t *label;
} MIPS_code_node_b_t;



typedef struct MIPS_code_line MIPS_code_line_t;
struct MIPS_code_line
{
    MIPS_code_node_t *node;
    MIPS_code_line_t *prev;
    MIPS_code_line_t *next;
};

typedef struct MIPS_code_list MIPS_code_list_t;
struct MIPS_code_list
{
    int size;
    MIPS_code_line_t *start;
    MIPS_code_line_t *end;
};
