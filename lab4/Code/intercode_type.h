#include "syntax_tree.h"

enum
{
    OPERAND_VARIABLE_V,
    OPERAND_VARIABLE_T,
    OPERAND_CONSTANT_I,
    OPERAND_CONSTANT_F,
    OPERAND_ADDRESS_V,
    OPERAND_ADDRESS_T
};
typedef struct operand operand_t;
struct operand
{
    int kind;
    union {
        int var_no;
        int value_int;
        float value_flt;
    };
};

operand_t *create_operand_const_int(int ival);
operand_t *create_operand_const_float(float fval);
operand_t *create_operand_const(TreeNode *node);
operand_t *create_operand_var(int operand_kind, int var_no);
void fprint_operand(operand_t *operand);
void fprint_operator(int operator_kind);

enum
{
    CODE_LABEL,
    CODE_FUNC,
    CODE_ASSIGN,
    CODE_OPERATOR,
    CODE_REF,
    CODE_DREF,
    CODE_DREF_ASSIGN,
    CODE_GOTO,
    CODE_IF_GOTO,
    CODE_RETURN,
    CODE_DEC,
    CODE_ARG,
    CODE_CALL,
    CODE_PARAM,
    CODE_READ,
    CODE_WRITE
};

enum
{
    OPERATOR_EQ,
    OPERATOR_NEQ,
    OPERATOR_L,
    OPERATOR_LE,
    OPERATOR_G,
    OPERATOR_GE,
    OPERATOR_PLS,
    OPERATOR_MIN,
    OPERATOR_STAR,
    OPERATOR_DIV
};

typedef struct intercode_node
{
    int kind;
} intercode_node_t;

typedef struct intercode_node_label
{
    int kind;
    int label;
} intercode_node_label_t;

typedef struct intercode_node_func
{
    int kind;
    const char *func_name;
} intercode_node_func_t;

typedef struct intercode_node_assign
{
    int kind;
    operand_t *left;
    operand_t *right;
} intercode_node_assign_t;

typedef struct intercode_node_binary
{
    int kind;
    int operator_kind;
    operand_t *target;
    operand_t *left;
    operand_t *right;
} intercode_node_binary_t;

typedef struct intercode_node_ref
{
    int kind;
    operand_t *left;
    operand_t *right;
} intercode_node_ref_t;

typedef struct intercode_node_dref
{
    int kind;
    operand_t *left;
    operand_t *right;
} intercode_node_dref_t;

typedef struct intercode_node_dref_assign
{
    int kind;
    operand_t *left;
    operand_t *right;
} intercode_node_dref_assign_t;

typedef struct intercode_node_goto
{
    int kind;
    int label;
} intercode_node_goto_t;

typedef struct intercode_node_if_goto
{
    int kind;
    int operator_kind;
    operand_t *if_left;
    const char *relop;
    operand_t *if_right;
    int target_label;
} intercode_node_if_goto_t;

typedef struct intercode_node_return
{
    int kind;
    int operand_kind;
    operand_t *ret;
} intercode_node_return_t;

typedef struct intercode_node_dec
{
    int kind;
    int var_no;
    int size;
} intercode_node_dec_t;

typedef struct intercode_node_arg
{
    int kind;
    int operand_kind;
    operand_t *arg;
} intercode_node_arg_t;

typedef struct intercode_node_call
{
    int kind;
    const char *call_name;
    operand_t *call_rtn;
} intercode_node_call_t;

typedef struct intercode_node_param
{
    int kind;
    int var_no;
} intercode_node_param_t;

typedef struct intercode_node_read
{
    int kind;
    operand_t *var;
} intercode_node_read_t;

typedef struct intercode_node_write
{
    int kind;
    operand_t *var;
} intercode_node_write_t;


intercode_node_t *create_intercode_node_label(int label);
intercode_node_t *create_intercode_node_func(const char *func_name);
intercode_node_t *create_intercode_node_assign(operand_t *left, operand_t *right);
intercode_node_t *create_intercode_node_binary(int operator_kind, operand_t *target, operand_t *left, operand_t *right);
intercode_node_t *create_intercode_node_ref(operand_t *left, operand_t *right);
intercode_node_t *create_intercode_node_dref(operand_t *left, operand_t *right);
intercode_node_t *create_intercode_node_dref_assign(operand_t *left, operand_t *right);
intercode_node_t *create_intercode_node_goto(int label);
intercode_node_t *create_intercode_node_if_goto(operand_t *if_left, const char *relop, operand_t *if_right, int target_label);
intercode_node_t *create_intercode_node_return(operand_t *ret);
intercode_node_t *create_intercode_node_dec(int var_no, int size);
intercode_node_t *create_intercode_node_arg(int operand_kind, operand_t *arg);
intercode_node_t *create_intercode_node_call(const char *call_name, operand_t *call_rtn);
intercode_node_t *create_intercode_node_param(int var_no);
intercode_node_t *create_intercode_node_read(operand_t *var);
intercode_node_t *create_intercode_node_write(operand_t *var);

void fprint_intercode_node(intercode_node_t *node);
void fprint_intercode_node_label(intercode_node_label_t *node);
void fprint_intercode_node_func(intercode_node_func_t *node);
void fprint_intercode_node_assign(intercode_node_assign_t *node);
void fprint_intercode_node_binary(intercode_node_binary_t *node);
void fprint_intercode_node_ref(intercode_node_ref_t *node);
void fprint_intercode_node_dref(intercode_node_dref_t *node);
void fprint_intercode_node_dref_assign(intercode_node_dref_assign_t *node);
void fprint_intercode_node_goto(intercode_node_goto_t *node);
void fprint_intercode_node_if_goto(intercode_node_if_goto_t *node);
void fprint_intercode_node_return(intercode_node_return_t *node);
void fprint_intercode_node_dec(intercode_node_dec_t *node);
void fprint_intercode_node_arg(intercode_node_arg_t *node);
void fprint_intercode_node_call(intercode_node_call_t *node);
void fprint_intercode_node_param(intercode_node_param_t *node);
void fprint_intercode_node_read(intercode_node_read_t *node);
void fprint_intercode_node_write(intercode_node_write_t *node);

const char *relop_not(const char *relop);

typedef struct intercode_line intercode_line_t;
struct intercode_line
{
    intercode_node_t *node;
    intercode_line_t *prev;
    intercode_line_t *next;
};

typedef struct intercode_list intercode_list_t;
struct intercode_list
{
    int size;
    intercode_line_t *start;
    intercode_line_t *end;
};

intercode_line_t *create_intercode_line(intercode_node_t *node);

void init_label_no();
void init_intercode_list();
void intercode_list_push_back(intercode_node_t *node);
void intercode_list_check_assign();

void fprint_intercode_list(FILE *fp);
intercode_line_t *get_intercode_list_start();
