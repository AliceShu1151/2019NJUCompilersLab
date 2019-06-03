enum { TYPE_BASIC, TYPE_ARRAY, TYPE_STRUCT, TYPE_FUNCTION };
enum { TYPE_BASIC_INT, TYPE_BASIC_FLOAT };

typedef struct type
{
    int type_kind;
} type_t;

typedef struct field_node
{
    type_t *type;
    const char *name;
    int lineno;
    struct field_node *next;
} field_node_t;

typedef struct field_list
{
    int size;
    field_node_t *start;
    field_node_t *end;
} field_list_t;

typedef struct type_node type_node_t;
struct type_node
{
    type_t *type;
    type_node_t *next_node;
};

typedef struct type_list
{
    int size;
    type_node_t *start;
    type_node_t *end;
} type_list_t;


typedef struct type_basic
{
    int type_kind;
    int int_or_float;
} type_basic_t;

typedef struct type_array
{
    int type_kind;
    int size;
    type_t *extend;
} type_array_t;

typedef struct type_struct
{
    int type_kind;
    const char *name;
    field_list_t *struct_fields;
} type_struct_t;

typedef struct type_func
{
    int type_kind;
    type_t *return_type;
    type_list_t *parameters;
} type_func_t;



// symbol
enum{NOT_DEFINED, DEFINED};
typedef struct symbol
{
    type_t *type;
    const char *name;
    int lineno;
    int is_defined;
    int is_param;
    int var_no;
} symbol_t;


// symbol_table
typedef struct symbol_table_node
{
    symbol_t *symbol;
    struct symbol_table_node *sibling;
    struct symbol_table_node *prev;
    struct symbol_table_node *next;
} st_node_t;

// environment stack
typedef struct environment_layer
{
    st_node_t *env_node_list;
    struct environment_layer *next_layer;
} env_layer_t;

// hash table
#define HASH_TABLE_SIZE 1000
typedef struct symbol_table
{
    env_layer_t *env_stack_top;
    st_node_t *h_table[HASH_TABLE_SIZE];
} symbol_table_t;


void init_type_list(type_list_t *type_list);
void init_hash_table(st_node_t **h_table);

type_basic_t *create_type_basic(int type);
type_array_t *create_type_array(int size, type_t *type);
type_struct_t *create_type_struct(const char *name, field_list_t *field_list);
type_func_t *create_type_func(type_t* rtn_type, type_list_t *param_list);

void init_symbol(symbol_t *symbol, const char *name, 
                        type_t *type, int lineno, int is_defined);
symbol_t *create_symbol();

field_node_t *create_field_node(const char* name, type_t *type, int lineno);
field_list_t *create_field_list();
type_node_t *create_type_node(type_t *type);
type_list_t *create_type_list();

void type_list_push_back(type_list_t *type_list, type_t *type);

void field_list_push_back(field_list_t *field_list, type_t *type, const char *name, int lineno);
void field_list_add_to_type_list(field_list_t *field_list, type_list_t *type_list);
field_node_t *field_list_find_name(field_list_t *struct_fields, const char *name);

st_node_t *create_st_node(symbol_t *symbol, st_node_t *old_st_node);
env_layer_t *create_env_layer();

unsigned int hash_pjw(const char *name);

enum {TYPE_NOT_EQUAL, TYPE_EQUAL};
int type_is_equal(type_t *type_1, type_t *type_2);
int type_basic_is_equal(type_basic_t *type_1, type_basic_t *type_2);
int type_array_is_equal(type_array_t *type_1, type_array_t *type_2);
int type_struct_is_equal(type_struct_t *type_1, type_struct_t *type_2);
int type_func_is_equal(type_func_t *type_1, type_func_t *type_2);
int type_type_list_is_equal(type_list_t *type_1, type_list_t *type_2);
int type_field_list_is_equal(field_list_t *type_1, field_list_t *type_2);

int type_is_int(type_t *type);

int sizeof_type(type_t *type);

void print_field_list(field_list_t *field_list);
void print_type_list(type_list_t *type_list);
void print_type_list_simple(type_list_t *type_list);

void print_type_basic(type_basic_t *type);
void print_type_array(type_array_t *type);
void print_type_array_simple(type_array_t *type);
void print_type_struct(type_struct_t *type);
void print_type_func(type_func_t *type);
void print_type(type_t *type);
void print_symbol(symbol_t *symbol);
void print_type_simple(type_t *type);