#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>
#include <stdarg.h>
#include <math.h>

#include "tree_expression.h"
#include "tree_dif_dump.h"
#include "read_expression.h"
#include "dif.h"
#include "simplify.h"

Node* copy_subtree(Node *node)
{
    if (node == NULL)
        return NULL;

    return new_node(copy_subtree(node->left), copy_subtree(node->right), node->value);
}

Node *take_derivative(Node *node)
{
    assert(node);

    switch (node->value.type)
    {
        case NODE_TYPE_NUM:
        {
            return NUM(0);
        }
        case NODE_TYPE_VAR:
        {
            return NUM(1);
        }
        case NODE_TYPE_OPER:
        {
            switch (node->value.data.operation)
            {
                case OPERATION_ADD:
                {
                    return ADD(dL, dR);
                }
                case OPERATION_SUB:
                {
                    return SUB(dL, dR);
                }
                case OPERATION_MUL:
                {
                    return ADD(MUL(dL, cR), MUL(dR, cL));
                }
                case OPERATION_DIV:
                {
                    return DIV (SUB(MUL(dL, cR), MUL(dR, cL)), MUL(cR, cR)) ;
                }
                case OPERATION_POWER:
                {
                    bool left = is_var_in_node(node->left);
                    bool right = is_var_in_node(node->right);
                    if (left && !right)
                    {
                        return MUL(cR, MUL(POW(cL, SUB(cR, NUM(1))), dL));
                    }
                }
                case OPERATION_SIN:
                {
                    return MUL(COS(cR), dR);
                }
                case OPERATION_COS:
                {
                    return MUL(MUL(NUM(-1), SIN(cR)), dR);
                }
                case OPERATION_LN:
                {
                    return MUL(DIV(NUM(1), cR), dR);
                }
                case OPERATION_UNKNOWN :
                default :
                    assert(0 && "not correct operation");

            }
        }

        default :
            assert(0 && "not correct type");

    }
    return NULL;
}


bool is_var_in_node (Node *node)
{
    assert(node);

    if (is_leaf(node))
    {
        if (node->value.type == NODE_TYPE_VAR)
        {
            return true;
        }
        else
            return false;
    }
    return  (node->left && is_var_in_node(node->left)) ||
            (node->right && is_var_in_node(node->right));
}

