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
    type_list_t parameters;
} type_func_t;



// symbol
enum{DEFINED, NOT_DEFINED};
typedef struct symbol
{
    type_t *type;
    const char *name;
    int lineno;
    int is_defined;
} symbol_t;


// symbol_table
enum{EMPTY, NOT_EMPTY};
typedef struct symbol_table_node
{
    int is_empyty;
    symbol_t symbol;
    struct symbol_table_node *sibling;
    struct symbol_table_node *prev;
    struct symbol_table_node *next;
} st_node_t;

// environment stack

typedef struct environment_node
{
    st_node_t *env_symbol_list;
    struct environment_node *next;
} env_node_t;


typedef struct enviroment_stack
{
    env_node_t *top;
} env_stack_t;

// hash table
#define HASH_table_SIZE 1000
typedef struct hash_table
{
    st_node_t ht_node[HASH_table_SIZE];
} hash_table_t;


typedef struct symbol_table
{
    env_stack_t env_stack;
    hash_table_t h_table;
} symbol_table_t;


void init_type_list(type_list_t *type_list);
void init_enviroment_stack(env_stack_t *env_stack);
void init_hash_table(hash_table_t *h_table);

type_basic_t *create_type_basic(int type);
type_array_t *create_array(int size, type_t *type);

type_struct_t *create_type_struct(const char *name, field_list_t *field_list);


field_node_t *create_field_node(const char* name, type_t *type);
field_list_t *create_field_list();

void print_field_list(field_list_t *field_list);
void print_type_basic(type_basic_t *type);
void print_type_array(type_array_t *type);
void print_type_struct(type_struct_t *type);
void print_type_func(type_func_t *type);
void print_type(type_t *type);
void print_symbol(symbol_t *symbol);