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
#include "tokenizator.h"
#include "descent_read.h"
#include"dump_lat.h"

//#define dump_to_lat();


Node *process_str (Node *root)
{
    assert(root);

    int index = 0;
    Node *node = GetG(root, &index);
    if (!node)
        printf ("Problem with read\n");

    node = optimize(node);
    if (!node)
        assert(0 && "Problem with optimize\n");

    return node;
}

Node* copy_subtree(Node *node)
{
    if (node == NULL)
        return NULL;

    return new_node(copy_subtree(node->left), copy_subtree(node->right), node->value);
}

Node *take_derivative(Node *node, FILE *p_file)
{
    assert(node);
    assert(p_file);

    print_node_to_lat(node, p_file);

    switch (node->value.type)
    {
        case NODE_TYPE_NUM:
        {
            Node *node_ = NUM(0);
            print_dif_node_to_lat(node_, p_file);
            return node_;
        }
        case NODE_TYPE_VAR:
        {
            Node *node_ = NUM(1);
            print_dif_node_to_lat(node_, p_file);
            return node_;
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
                case OPERATION_OPEN_BRACKET:
                case OPERATION_CLOSE_BRACKET:
                case OPERATION_END:
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

