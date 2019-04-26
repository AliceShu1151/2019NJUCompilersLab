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
    "\"%s\" is not an array.\n",
    "\"%s\" is not a function.\n",
    "\"%s\" is not an integer.\n",
    "Illegal use of \".\".\n",
    "Non-existent field \"%s\".\n",
    "Redefined field \"%s\" or Field assigned during definition.\n",
    "Duplicated name \"%s\".\n",
    "Undefined structure \"%s\".\n",
    "Undefined function \"%s\".\n",
    "Inconsistent declaration of function \"%s\".\n"
};

void print_semantic_error(int error_type, int lineno,...)
{
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
    symbol_table.env_stack_top = init_enviroment_stack();
    init_hash_table(&symbol_table.h_table);
}

int type_struct_equal(type_struct_t *struct_type, const char *struct_name)
{
    assert(struct_type->type_kind == TYPE_STRUCT);
    if (strcmp(struct_type->name, struct_name) == 0) 
    {
        return 1;
    }
    return 0;
}


void field_list_push_back(field_list_t *field_list, type_t *type, const char *name)
{
    field_node_t *field_node = create_field_node(name, type);
    if (field_list->start == NULL) {
        field_list->size = field_list->size + 1;
        field_list->start = field_node;
        field_list->end = field_node;
    }
    else
    {
        field_list->size = field_list->size + 1;
        field_list->end->next = field_node;
        field_list->end = field_node;
    }
}


void struct_table_push_back(type_struct_t *new_struct)
{
    type_node_t *node = malloc(sizeof(type_node_t));
    node->type = (type_t *)new_struct;
    node->next_node = NULL;
    if (struct_type_table.start == NULL) {
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
    if (struct_type_table.size == 0) {
        return NULL;
    }
    type_node_t *itor = struct_type_table.start;
    while (itor != NULL)
    {
        if (type_struct_equal((type_struct_t *)itor->type, struct_name)) {
            return itor->type;
        }   
        itor = itor->next_node;
    }
    return NULL;
}

st_node_t *env_layer_find_name(st_node_t *node_list, const char *name)
{
    for(st_node_t *itor = node_list; itor != NULL; itor = itor->sibling)
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

symbol_t *symbol_table_find_name(symbol_t *symbol)
{
    st_node_t *rtn;
    for(env_layer_t *itor = symbol_table.env_stack_top; itor != NULL; itor = itor->next_layer)
    {
        rtn = env_layer_find_name(itor->env_node_list, symbol->name);
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




void print_struct_type_table()
{
    type_node_t *itor;
    for(itor = struct_type_table.start; itor != NULL; itor = itor->next_node)
    {
        print_type(itor->type);
    }
}



void print_env_layer(st_node_t *node_list)
{
    for(st_node_t *itor = node_list->next; itor != NULL; itor = itor->sibling)
    {
        print_symbol(itor->symbol);
    }
    printf("\n");
}

void print_symbol_table()
{
    for(env_layer_t *itor = symbol_table.env_stack_top; itor != NULL; itor = itor->next_layer)
    {
        print_env_layer(itor->env_node_list);
    }
}