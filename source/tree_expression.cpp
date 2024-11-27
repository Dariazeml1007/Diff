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

Node * new_node (Node *left, Node *right, Node_value val)
{

    struct Node *node = (Node *) calloc (1, sizeof(Node));
    if (!node)
        return NULL;

    node->value = val;
    node->right = right;
    node->left = left;

    if (left)
        left->parent = node;

    if (right)
        right->parent = node;


    switch(node->value.type)
    {
        case NODE_TYPE_OPER:
        {
            node->value.data.operation = val.data.operation;
            break;
        }

        case NODE_TYPE_NUM:
        {
            node->value.data.number = val.data.number;
            printf ("new_node_func value of num = %lf\n", node->value.data.number);
            break;
        }

        case NODE_TYPE_VAR:
        {
            node->value.data.variable = val.data.variable;
            printf ("new_node_func value of var = %c\n", node->value.data.variable);
            break;
        }

        default :
            assert(0 && "NOT CORRECT TYPE");

    }


    return node;

}

void print_tree (Node *node)
{

    if (node == NULL)
    {
        printf ("NULL pointer\n");
        return;
    }

    if (node->left)
        print_tree(node->left);

    if (is_leaf(node))
    {
        if (node->value.type == NODE_TYPE_NUM)
            printf ("%lf", node->value.data.number);
        else if (node->value.type == NODE_TYPE_VAR)
            printf ("%c ", node->value.data.variable);
    }
    else
    {
        printf ("%s", array_of_oper[node->value.data.operation].str_operation);
    }

    if (node->right)
        print_tree(node->right);

}

bool is_leaf(Node *node)
{
    assert(node);

    return !node->left && !node->right;
}

void node_dtor (Node *node)
{
    assert(node);

    if (node->right)
    {
        if (!is_leaf(node->right))
            node_dtor(node->right);

        else
        {
            free(node->right);

        }
    }

    if (node->left)
    {
        if (!is_leaf(node->left))
            node_dtor(node->left);

        else
        {
            free(node->left);

        }
    }



    free(node);
    return;
}

