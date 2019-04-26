#include "type.h"
#include <stdlib.h>
#include <stdio.h>

void init_type_list(type_list_t *type_list)
{
    type_list->size = 0;
    type_list->start = NULL;
    type_list->end = NULL;
}


env_node_t *init_enviroment_stack()
{
    env_node_t *rtn = malloc(sizeof(env_node_t));
    rtn->env_symbol_list = NULL;
    rtn->next_layer = NULL;
    return rtn;
}

void init_hash_table(st_node_t *h_table)
{
    for(size_t i = 0; i < HASH_TABLE_SIZE; i++)
    {
        h_table[i] = NULL;
    }
}


type_basic_t *create_type_basic(int type)
{
    type_basic_t *rtn = malloc(sizeof(type_basic_t));
    rtn->type_kind = TYPE_BASIC;
    rtn->int_or_float = type;
    return rtn;
}

type_array_t *create_array(int size, type_t *type)
{
    type_array_t *rtn = malloc(sizeof(type_array_t));
    rtn->type_kind = TYPE_ARRAY;
    rtn->size = size;
    rtn->extend = type;
    return rtn;
}

type_struct_t *create_type_struct(const char *name, field_list_t *field_list)
{
    type_struct_t *rtn = malloc(sizeof(type_struct_t));
    rtn->type_kind = TYPE_STRUCT;
    rtn->name = name;
    rtn->struct_fields = field_list;
    return rtn;
}

type_func_t *create_type_func(type_t* rtn_type, field_list_t *param_list)
{
    type_func_t *rtn = malloc(sizeof(type_func_t));
    rtn->type_kind = TYPE_FUNCTION;
    rtn->return_type = rtn_type;
    rtn->parameters = param_list;
    return rtn;
}

void init_symbol(symbol_t *symbol, const char *name, type_t *type, int lineno, int is_defined)
{
    symbol->name = name;
    symbol->lineno = lineno;
    symbol->type = type;
    symbol->is_defined = is_defined;
}

field_node_t *create_field_node(const char* name, type_t *type)
{
    field_node_t *rtn = malloc(sizeof(field_node_t));
    rtn->name = name;
    rtn->type = type;
    rtn->next = NULL;
    return rtn;
}

field_list_t *create_field_list()
{
    field_list_t *rtn = malloc(sizeof(field_list_t));
    rtn->size = 0;
    rtn->start = NULL;
    rtn->end =NULL;
    return rtn; 
}

st_node_t *create_st_node(symbol_t *symbol, st_node_t *old_st_node)
{
    if (old_st_node->is_empty == EMPTY) 
    {
        old_st_node->symbol = symbol;
        old_st_node->is_empty =NOT_EMPTY;   
        return old_st_node;    
    }
    else
    {
        st_node_t *rtn = malloc(sizeof(st_node_t));
        
    }
    
    
}


unsigned int hash_pjw(const char *name)
{
    unsigned int val = 0, i;
    for(const char *itor = name; *itor; ++itor)
    {
        val = (val << 2) + *itor;
        if (i == val & ~HASH_TABLE_SIZE) {
            val = (val ^ (i >> 12)) & HASH_TABLE_SIZE;
        }
    }
    return val;
}

void print_field_list(field_list_t *field_list)
{
    if (field_list == NULL) {
        return;
    }
    field_node_t *itor;
    for (itor = field_list->start; itor != NULL; itor = itor->next)
    {
        printf("%s: ", itor->name);
        print_type(itor->type);
        printf("\n");
    }
}

void print_type_basic(type_basic_t *type)
{
    if (type->int_or_float == TYPE_BASIC_INT) 
    {
        printf("int");
    }
    else if (type->int_or_float == TYPE_BASIC_FLOAT) 
    {
        printf("float");
    }
}

void print_type_array(type_array_t *type)
{
    printf("array[%d]", type->size);
    print_type(type->extend);
}

void print_type_struct(type_struct_t *type)
{
    printf("struct: %s\n", type->name);
    print_field_list(type->struct_fields);
}

void print_type_func(type_func_t *type)
{
    printf("function:\n");
    print_type(type->return_type);
    //print_type_list(type->parameters);
}

void print_type(type_t *type)
{
    if (type == NULL) 
    {
        return;
    }
    if (type->type_kind == TYPE_BASIC) 
    {
        print_type_basic((type_basic_t *)type);
    }
    else if (type->type_kind == TYPE_ARRAY) 
    {
        print_type_array((type_array_t *)type);
    }
    else if (type->type_kind == TYPE_STRUCT)
    {
        print_type_struct((type_struct_t *)type);
    }
    else if (type->type_kind == TYPE_FUNCTION) 
    {
        print_type_func((type_func_t *)type);
    } 
}

void print_symbol(symbol_t *symbol)
{
    printf("%s:", symbol->name);
    print_type(symbol->type);
    printf("\n");
}