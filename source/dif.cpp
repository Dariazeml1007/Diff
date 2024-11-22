#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>
#include <stdarg.h>

#include "tree_expression.h"
#include "tree_dif_dump.h"
#include "dif.h"

#define _NUM(a) new_node(NODE_TYPE_NUM, NULL, NULL, a)//ADD
#define _x new_node(NODE_TYPE_VAR, NULL, NULL,  'x' )//N_NODE
#define _ADD(a, b) new_node( NODE_TYPE_OPER, a, b, OPERATION_ADD)
#define _SUB(a, b) new_node(NODE_TYPE_OPER, a, b, OPERATION_SUB)
#define _MUL(a, b) new_node(NODE_TYPE_OPER, a, b, OPERATION_MUL)
#define _DIV(a, b) new_node(NODE_TYPE_OPER, a, b, OPERATION_DIV)

Node *take_derivative(Node *node)
{
    assert(node);

    switch (node->type)
    {
        case NODE_TYPE_NUM:
        {
            return _NUM(double(0));;
        }
        case NODE_TYPE_VAR:
        {
            return _NUM(double(1));
        }
        case NODE_TYPE_OPER:
        {
            switch (node->value)
            {
                case OPERATION_ADD:
                {
                    Node *dL = take_derivative(node->left);
                    Node *dR = take_derivative(node->right);
                    return _ADD(dL, dR);

                }
                case OPERATION_SUB:
                {
                    Node *dL = take_derivative(node->left);
                    Node *dR = take_derivative(node->right);
                    return _SUB(dL, dR);

                }
                default :
                    assert(0 && "not correct operation");

            }
        }
        default :
            assert(0 && "not correct type");

    }
    return NULL;
}

#undef _NUM
#undef _x
#undef _ADD
#undef _SUB
#undef _MUL
#undef _DIV
