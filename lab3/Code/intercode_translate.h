#include "semantic_analysis.h"
#include "intercode_type.h"

typedef struct intercode_line intercode_line_t;
struct intercode_line
{
    intercode_node_t *node;
    intercode_line_t *prev;
    intercode_line_t *next;
};

typedef struct intercode_list intercode_list_t;
struct intercode_list
{
    int size;
    intercode_line_t *start;
    intercode_line_t *end;
};



void intercode_translate(TreeNode *root);