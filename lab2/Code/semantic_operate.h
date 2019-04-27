#include "type.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void init_struct_type_table();
void init_symbol_table();


void print_semantic_error(int error_type, int lineno,...);
void print_struct_type_table();
void print_env_layer(st_node_t *node_list);
void print_symbol_table();

void field_list_push_back(field_list_t *field_list, type_t *type, const char *name, int lineno);

void struct_table_push_back(type_struct_t *new_struct);
type_t *struct_table_find_name(const char *struct_name);

void symbol_table_add(symbol_t * symbol);
void symbol_table_del(unsigned int hash_num, st_node_t *node);
symbol_t *symbol_table_find_name(const char *name);

st_node_t *env_layer_find_name(st_node_t *node_list, const char *name);
st_node_t *top_env_layer_find_name(const char *name);

void symbol_table_env_stack_push();
void symbol_table_env_stack_pop();

void param_list_add_env_layer(field_list_t *param_list);