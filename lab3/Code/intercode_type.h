enum
{
    OPERAND_VARIABLE,
    OPERAND_CONSTANT,
    OPERAND_ADDRESS
};
typedef struct operand operand_t;
struct operand
{
    int kind;
    union {
        int var_no;
        int value;
        int addr;
    };
};

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
    operand_t right;
    operand_t left;
} intercode_node_assign_t;

typedef struct intercode_node_binary
{
    int kind;
    int operator_kind;
    operand_t target;
    operand_t left;
    operand_t right;
} intercode_node_binary_t;

typedef struct intercode_node_ref
{
    int kind;
    operand_t left;
    operand_t right;
} intercode_node_ref_t;

typedef struct intercode_node_dref
{
    int kind;
    operand_t left;
    operand_t right;
} intercode_node_dref_t;

typedef struct intercode_node_dref_assign
{
    int kind;
    operand_t left;
    operand_t right;
} intercode_node_dref_assign_t;

typedef struct intercode_node_goto
{
    int kind;
    int label;
} intercode_node_goto_t;

typedef struct intercode_node_if_goto
{
    int kind;
    operand_t if_left;
    operand_t if_right;
    int target;
} intercode_node_if_goto_t;

typedef struct intercode_node_return
{
    int kind;
    operand_t rtn;
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
    int var_no;
} intercode_node_arg_t;

typedef struct intercode_node_call
{
    int kind;
    const char *call_name;
    operand_t call_rtn;
} intercode_node_call_t;

typedef struct intercode_node_param
{
    int kind;
    int var_no;
} intercode_node_param_t;

typedef struct intercode_node_read
{
    int kind;
    int var_no;
} intercode_node_read_t;

typedef struct intercode_node_write
{
    int kind;
    int var_no;
} intercode_node_write_t;
