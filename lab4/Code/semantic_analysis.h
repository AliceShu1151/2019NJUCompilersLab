#include "syntax_tree.h"
#include "semantic_operate.h"

enum {STRUCT_SPECIFIER, STATEMENT};

void semantic_analysis(TreeNode *root);

void semantic_extdef(TreeNode *node);

void semantic_extdeclist(TreeNode *node, type_t *type);
type_t *semantic_specifier(TreeNode *node);
type_t *semantic_structspecifier(TreeNode *node);
void semantic_deflist(TreeNode *node, int where, field_list_t *field_list);
void semantic_declist(TreeNode *node, type_t *type, field_list_t *field_list, int where);
void semantic_dec(TreeNode *node, type_t *type, field_list_t *field_list, int where);
void semantic_vardec(TreeNode *node, symbol_t *symbol, type_t *type);

void semantic_fundec(TreeNode *node, type_t *type, symbol_t *symbol, field_list_t *field_list);
void semantic_varlist(TreeNode *node, field_list_t *field_list);
void semantic_paramdec(TreeNode *node, field_list_t *field_list);

void semantic_compst(TreeNode *node, type_t *rtn_type);
void semantic_stmtlist(TreeNode *node, type_t *rtn_type);
void semantic_stmt(TreeNode *node, type_t *rtn_type);
void semantic_args(TreeNode *node, type_list_t *type_list);

int symbol_table_check_add(symbol_t *symbol);
int symbol_table_check_add_func(symbol_t *symbol_func, int is_define);
void symbol_table_check_undefined();

void field_list_check_push_back(field_list_t *field_list, symbol_t *symbol);

void param_list_check_add(symbol_t *symbol, field_list_t *param_list);

void compst_env_init(symbol_t *symbol, field_list_t *param_list);

int has_semantic_error();