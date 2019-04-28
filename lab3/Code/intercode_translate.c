#include "intercode_translate.h"

void intercode_translate_r(TreeNode *node);


void intercode_translate(TreeNode *root)
{
    init_struct_type_table();
    init_symbol_table();
    intercode_translate_r(root);
}

void intercode_translate_r(TreeNode *node)
{
    if (node == NULL)
    {
        return;
    }

    if (strcmp(node->tokenname, "ExtDef") == 0)
    {
        //translate_extdef(node);
        return;
    }

    for (TreeNode *child = node->child; child != NULL; child = child->brother)
    {
        intercode_translate_r(child);
    }
}
