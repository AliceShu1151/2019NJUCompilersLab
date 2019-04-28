#include "syntax_tree.h"
#include "semantic_operate.h"

enum {STRUCT_SPECIFIER, STATEMENT};

void semantic_analysis(TreeNode *root);

void symbol_table_check_undefined();

int has_semantic_error();