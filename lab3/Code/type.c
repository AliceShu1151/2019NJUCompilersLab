#include "type.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void init_type_list(type_list_t *type_list)
{
    type_list->size = 0;
    type_list->start = NULL;
    type_list->end = NULL;
}

void init_hash_table(st_node_t **h_table)
{
    memset(h_table, 0, sizeof(st_node_t *) * HASH_TABLE_SIZE);
}

type_basic_t *create_type_basic(int type)
{
    type_basic_t *rtn = malloc(sizeof(type_basic_t));
    rtn->type_kind = TYPE_BASIC;
    rtn->int_or_float = type;
    return rtn;
}

type_array_t *create_type_array(int size, type_t *type)
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

type_func_t *create_type_func(type_t *rtn_type, type_list_t *param_list)
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

field_node_t *create_field_node(const char *name, type_t *type, int lineno)
{
    field_node_t *rtn = malloc(sizeof(field_node_t));
    rtn->name = name;
    rtn->lineno = lineno;
    rtn->type = type;
    rtn->next = NULL;
    return rtn;
}

field_list_t *create_field_list()
{
    field_list_t *rtn = malloc(sizeof(field_list_t));
    rtn->size = 0;
    rtn->start = NULL;
    rtn->end = NULL;
    return rtn;
}

symbol_t *create_symbol()
{
    symbol_t *rtn = malloc(sizeof(symbol_t));
    rtn->is_defined = NOT_DEFINED;
    rtn->type = NULL;
    return rtn;
}

type_node_t *create_type_node(type_t *type)
{
    type_node_t *rtn = malloc(sizeof(type_node_t));
    rtn->type = type;
    return rtn;
}

type_list_t *create_type_list()
{
    type_list_t *rtn = malloc(sizeof(type_list_t));
    rtn->size = 0;
    rtn->start = NULL;
    rtn->end = NULL;
    return rtn;
}

void type_list_push_back(type_list_t *type_list, type_t *type)
{
    type_node_t *type_node = create_type_node(type);
    if (type_list->start == NULL)
    {
        type_list->start = type_node;
    }
    else
    {
        type_list->end->next_node = type_node;
    }
    type_list->size = type_list->size + 1;
    type_list->end = type_node;
}

void field_list_push_back(field_list_t *field_list, type_t *type, const char *name, int lineno)
{
    field_node_t *field_node = create_field_node(name, type, lineno);
    if (field_list->start == NULL)
    {
        field_list->start = field_node;
    }
    else
    {
        field_list->end->next = field_node;
    }
    field_list->size = field_list->size + 1;
    field_list->end = field_node;
}

void field_list_add_to_type_list(field_list_t *field_list, type_list_t *type_list)
{
    for (field_node_t *itor = field_list->start; itor != NULL; itor = itor->next)
    {
        type_node_t *type_node = create_type_node(itor->type);
        if (type_list->size == 0)
        {
            type_list->start = type_node;
        }
        else
        {
            type_list->end->next_node = type_node;
        }
        type_list->end = type_node;
        type_list->size++;
    }
}

field_node_t *field_list_find_name(field_list_t *struct_fields, const char *name)
{
    field_node_t *itor;
    for (itor = struct_fields->start; itor != NULL; itor = itor->next)
    {
        if (strcmp(itor->name, name) == 0)
        {
            return itor;
        }
    }
    return NULL;
}

st_node_t *create_st_node(symbol_t *symbol, st_node_t *old_st_node)
{
    st_node_t *rtn = malloc(sizeof(st_node_t));
    rtn->symbol = symbol;
    rtn->next = NULL;
    rtn->prev = NULL;
    rtn->sibling = NULL;
    if (old_st_node != NULL)
    {
        old_st_node->prev = rtn;
        rtn->next = old_st_node;
    }
    return rtn;
}

env_layer_t *create_env_layer()
{
    env_layer_t *rtn = malloc(sizeof(env_layer_t));
    rtn->env_node_list = NULL;
    rtn->next_layer = NULL;
    return rtn;
}

unsigned int hash_pjw(const char *name)
{
    unsigned int val = 0, i;
    for (const char *itor = name; *itor; ++itor)
    {
        val = (val << 2) + *itor;
        if ((i = val & ~0x3fff))
        {
            val = (val ^ (i >> 12)) & 0x3fff;
        }
    }
    return val;
}

int type_is_equal(type_t *type_1, type_t *type_2)
{
    if (type_1->type_kind != type_2->type_kind)
    {
        return TYPE_NOT_EQUAL;
    }
    int type_kind = type_1->type_kind;
    if (type_kind == TYPE_BASIC)
    {
        return type_basic_is_equal((type_basic_t *)type_1, (type_basic_t *)type_2);
    }
    else if (type_kind == TYPE_ARRAY)
    {
        return type_array_is_equal((type_array_t *)type_1, (type_array_t *)type_2);
    }
    else if (type_kind == TYPE_STRUCT)
    {
        return type_struct_is_equal((type_struct_t *)type_1, (type_struct_t *)type_2);
    }
    else if (type_kind == TYPE_FUNCTION)
    {
        return type_func_is_equal((type_func_t *)type_1, (type_func_t *)type_2);
    }
    return TYPE_NOT_EQUAL;
}

int type_basic_is_equal(type_basic_t *type_1, type_basic_t *type_2)
{
    if (type_1->int_or_float == type_2->int_or_float)
    {
        return TYPE_EQUAL;
    }
    else
    {
        return TYPE_NOT_EQUAL;
    }
}
int type_array_is_equal(type_array_t *type_1, type_array_t *type_2)
{
    return type_is_equal(type_1->extend, type_2->extend);
}
int type_struct_is_equal(type_struct_t *type_1, type_struct_t *type_2)
{
    return type_field_list_is_equal((field_list_t *)type_1->struct_fields, (field_list_t *)type_2->struct_fields);
}
int type_func_is_equal(type_func_t *type_1, type_func_t *type_2)
{
    if (type_is_equal(type_1->return_type, type_2->return_type) == TYPE_NOT_EQUAL)
    {
        return TYPE_NOT_EQUAL;
    }
    return type_type_list_is_equal(type_1->parameters, type_2->parameters);
}
int type_type_list_is_equal(type_list_t *type_1, type_list_t *type_2)
{
    if (type_1->size != type_2->size)
    {
        return TYPE_NOT_EQUAL;
    }
    type_node_t *itor_1 = type_1->start;
    type_node_t *itor_2 = type_2->start;
    for (; itor_1 != NULL;)
    {
        if (type_is_equal(itor_1->type, itor_2->type) == TYPE_NOT_EQUAL)
        {
            return TYPE_NOT_EQUAL;
        }
        itor_1 = itor_1->next_node;
        itor_2 = itor_2->next_node;
    }
    return TYPE_EQUAL;
}
int type_field_list_is_equal(field_list_t *type_1, field_list_t *type_2)
{
    // just type, do not care name
    if (type_1->size != type_2->size)
    {
        return TYPE_NOT_EQUAL;
    }
    field_node_t *itor_1 = type_1->start;
    field_node_t *itor_2 = type_2->start;
    for (; itor_1 != NULL;)
    {
        if (type_is_equal(itor_1->type, itor_2->type) == TYPE_NOT_EQUAL)
        {
            return TYPE_NOT_EQUAL;
        }
        itor_1 = itor_1->next;
        itor_2 = itor_2->next;
    }
    return TYPE_EQUAL;
}

int type_is_int(type_t *type)
{
    if (type->type_kind == TYPE_BASIC)
    {
        if (((type_basic_t *)type)->int_or_float == TYPE_BASIC_INT)
        {
            return 1;
        }
    }
    return 0;
}

void print_field_list(field_list_t *field_list)
{
    if (field_list == NULL)
    {
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

void print_type_list(type_list_t *type_list)
{
    if (type_list->size == 0)
    {
        return;
    }
    type_node_t *itor = type_list->start;
    print_type(itor->type);
    for (itor = itor->next_node; itor != NULL; itor = itor->next_node)
    {
        printf(",");
        print_type(itor->type);
    }
}

void print_type_list_simple(type_list_t *type_list)
{
    if (type_list->size == 0)
    {
        return;
    }
    type_node_t *itor = type_list->start;
    print_type_simple(itor->type);
    for (itor = itor->next_node; itor != NULL; itor = itor->next_node)
    {
        printf(",");
        print_type_simple(itor->type);
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

void print_type_array_simple(type_array_t *type)
{
    printf("array[%d]", type->size);
    print_type_simple(type->extend);
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
    print_type_list(type->parameters);
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

void print_type_simple(type_t *type)
{
    if (type->type_kind == TYPE_BASIC)
    {
        print_type_basic((type_basic_t *)type);
    }
    else if (type->type_kind == TYPE_ARRAY)
    {
        print_type_array_simple((type_array_t *)type);
    }
    else if (type->type_kind == TYPE_STRUCT)
    {
        printf("struct:%s", ((type_struct_t *)type)->name);
    }
}

void print_symbol(symbol_t *symbol)
{
    printf("%s:", symbol->name);
    print_type(symbol->type);
    printf("\n");
}