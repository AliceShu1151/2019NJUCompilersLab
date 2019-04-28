#include "semantic_operate.h"

static type_list_t struct_type_table;
static symbol_table_t symbol_table;
static int semantic_error = 0;
static char *error_msg[20] = {
    ".",
    "Undefined variable \"%s\".\n",
    "Undifined function \"%s\".\n",
    "Redifined variable \"%s\".\n",
    "Redifined function \"%s\".\n",
    "Type mismatched for assighment.\n",
    "The left-hand side of an assighment must be a variable.\n",
    "Type mismatched for operands.\n",
    "Type mismatched for return.\n",
    "Function \"%s\" is not applicable for arguments \"%s\".\n",
    "This is not an array.\n",
    "\"%s\" is not a function.\n",
    "This is not an integer.\n",
    "Illegal use of \".\".\n",
    "Non-existent field \"%s\".\n",
    "Redefined field \"%s\" or Field assigned during definition.\n",
    "Duplicated name \"%s\".\n",
    "Undefined structure \"%s\".\n",
    "Undefined function \"%s\".\n",
    "Inconsistent declaration of function \"%s\".\n"
    };

int type_struct_equal_find_name(type_struct_t *struct_type, const char *struct_name);
void env_layer_add_node(st_node_t *new_node);

void print_semantic_error(int error_type, int lineno, ...)
{
    semantic_error = 1;
    printf("Error type %d at Line %d: ", error_type, lineno);
    va_list ap;
    va_start(ap, lineno);
    vprintf(error_msg[error_type], ap);
    va_end(ap);
}

void init_struct_type_table()
{
    init_type_list(&struct_type_table);
}

void init_symbol_table()
{
    symbol_table_env_stack_push();
    init_hash_table(symbol_table.h_table);
}

int type_struct_equal_find_name(type_struct_t *struct_type, const char *struct_name)
{
    assert(struct_type->type_kind == TYPE_STRUCT);
    if (strcmp(struct_type->name, struct_name) == 0)
    {
        return 1;
    }
    return 0;
}

void struct_table_push_back(type_struct_t *new_struct)
{
    type_node_t *node = malloc(sizeof(type_node_t));
    node->type = (type_t *)new_struct;
    node->next_node = NULL;
    if (struct_type_table.start == NULL)
    {
        struct_type_table.start = node;
    }
    else
    {
        struct_type_table.end->next_node = node;
    }
    struct_type_table.end = node;
    struct_type_table.size++;
}

type_t *struct_table_find_name(const char *struct_name)
{
    if (struct_type_table.size == 0)
    {
        return NULL;
    }
    type_node_t *itor = struct_type_table.start;
    while (itor != NULL)
    {
        if (type_struct_equal_find_name((type_struct_t *)itor->type, struct_name))
        {
            return itor->type;
        }
        itor = itor->next_node;
    }
    return NULL;
}

st_node_t *env_layer_find_name(st_node_t *node_list, const char *name)
{
    for (st_node_t *itor = node_list; itor != NULL; itor = itor->sibling)
    {
        if (strcmp(itor->symbol->name, name) == 0)
        {
            return itor;
        }
    }
    return NULL;
}

st_node_t *top_env_layer_find_name(const char *name)
{
    return env_layer_find_name(symbol_table.env_stack_top->env_node_list, name);
}

symbol_t *symbol_table_find_name(const char *name)
{
    st_node_t *rtn;
    for (env_layer_t *itor = symbol_table.env_stack_top; itor != NULL; itor = itor->next_layer)
    {
        rtn = env_layer_find_name(itor->env_node_list, name);
        if (rtn != NULL)
        {
            return rtn->symbol;
        }
    }
    return NULL;
}

void env_layer_add_node(st_node_t *new_node)
{
    if (symbol_table.env_stack_top->env_node_list == NULL)
    {
        symbol_table.env_stack_top->env_node_list = new_node;
    }
    else
    {
        new_node->sibling = symbol_table.env_stack_top->env_node_list;
        symbol_table.env_stack_top->env_node_list = new_node;
    }
}

void symbol_table_add(symbol_t *symbol)
{
    unsigned int hash_num = hash_pjw(symbol->name) % HASH_TABLE_SIZE;
    symbol_table.h_table[hash_num] = create_st_node(symbol, symbol_table.h_table[hash_num]);
    env_layer_add_node(symbol_table.h_table[hash_num]);
}

void symbol_table_del(unsigned int hash_num, st_node_t *node)
{
    if (node->prev == NULL)
    {
        symbol_table.h_table[hash_num] = node->next;
    }
    else
    {
        node->prev->next = node->next;
    }
    if (node->next != NULL)
    {
        node->next->prev = node->prev;
    }
}

void symbol_table_env_stack_push()
{
    env_layer_t *new_layer = create_env_layer();
    if (symbol_table.env_stack_top == NULL)
    {
        symbol_table.env_stack_top = new_layer;
        return;
    }
    new_layer->next_layer = symbol_table.env_stack_top;
    symbol_table.env_stack_top = new_layer;
}

void symbol_table_env_stack_pop()
{
    if (symbol_table.env_stack_top == NULL)
    {
        return;
    }
    for (st_node_t *itor = symbol_table.env_stack_top->env_node_list; itor != NULL; itor = itor->sibling)
    {
        unsigned int hash_num = hash_pjw(itor->symbol->name) % HASH_TABLE_SIZE;
        symbol_table_del(hash_num, itor);
    }
    symbol_table.env_stack_top = symbol_table.env_stack_top->next_layer;
}

void print_struct_type_table()
{
    type_node_t *itor;
    for (itor = struct_type_table.start; itor != NULL; itor = itor->next_node)
    {
        print_type(itor->type);
    }
}

void env_layer_check_undefined(st_node_t *node_list)
{
    for (st_node_t *itor = node_list; itor != NULL; itor = itor->sibling)
    {
        if (itor->symbol->is_defined == NOT_DEFINED) {
            print_semantic_error(18, itor->symbol->lineno, itor->symbol->name);
        }
    }
}

void symbol_table_check_undefined()
{
    for (env_layer_t *itor = symbol_table.env_stack_top; itor != NULL; itor = itor->next_layer)
    {
        env_layer_check_undefined(itor->env_node_list);
    }
}

void print_env_layer(st_node_t *node_list)
{
    for (st_node_t *itor = node_list; itor != NULL; itor = itor->sibling)
    {
        print_symbol(itor->symbol);
    }
    printf("\n");
}

void print_symbol_table()
{
    for (env_layer_t *itor = symbol_table.env_stack_top; itor != NULL; itor = itor->next_layer)
    {
        print_env_layer(itor->env_node_list);
    }
}

void param_list_add_env_layer(field_list_t *param_list)
{
    for(field_node_t *itor = param_list->start; itor != NULL; itor = itor->next)
    {
        symbol_t *symbol = create_symbol();
        init_symbol(symbol, itor->name, itor->type, itor->lineno, DEFINED);
        symbol_table_add(symbol);
    }
}