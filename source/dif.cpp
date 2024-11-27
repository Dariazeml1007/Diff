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


Node* copy_subtree(Node *node);
// #define VAR new_node(NODE_TYPE_VAR, NULL, NULL,  'x' )//N_NODE
//{.type = NODE_TYPE_NUM},
#define NUM(a) new_node(NULL, NULL, (struct Node_value){.type = NODE_TYPE_NUM, .data = {.number = a}})
#define ADD(a, b) new_node(a, b, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_ADD}})
#define SUB(a, b) new_node(a, b, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_SUB}})
#define MUL(a, b) new_node(a, b, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_MUL}})
#define DIV(a, b) new_node(a, b, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_DIV}})
#define dL take_derivative(node->left)
#define dR take_derivative(node->right)
#define cL copy_subtree(node->left)
#define cR copy_subtree(node->right)

Node* copy_subtree(Node *node)
{
    if(node == NULL)
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
            return NUM(0); //NUM(0)
        }
        case NODE_TYPE_VAR:
        {
            printf ("dif var\n");
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
                case OPERATION_UNKNOWN :
                {
                    assert(0 && "unknown  operation");
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

#undef NUM
#undef ADD
#undef SUB
#undef MUL
#undef DIV

// #undef VAR
