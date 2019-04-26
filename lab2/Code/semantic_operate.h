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

void field_list_push_back(field_list_t *field_list, type_t *type, const char *name);

void struct_table_push_back(type_struct_t *new_struct);
type_t *struct_table_find_name(const char *struct_name);

void symbol_table_check_add(symbol_t *symbol);