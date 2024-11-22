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

Node * new_node (Info_t type, Node *left, Node *right, ...)
{


    va_list factor;
    va_start(factor, right);

    struct Node *node = (Node *) calloc (1, sizeof(Node));
    if (!node)
        printf ("Allocation memory\n");

    node->type = type;


    node->right = right;
    node->left = left;

    if (left)
        left->parent = node;

    if (right)
        right->parent = node;


    switch(type)
    {
        case NODE_TYPE_OPER:
        {
            operation_t value = (operation_t) va_arg(factor, int);
            *(operation_t *)&node->value = value;
            break;
        }

        case NODE_TYPE_NUM:
        {
            double value = (double) va_arg(factor, double);
            *(double *)&node->value = value;
            printf ("new_node_func value of num = %lf\n", value);
            break;
        }

        case NODE_TYPE_VAR:
        {
            char value = (char) va_arg(factor, int);
            *(char *)&node->value = value;
            printf ("new_node_func value of var = %c\n", value);
            break;
        }

        default :
            assert(0 && "NOT CORRECT TYPE");

    }

    va_end(factor);

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
        if (node->type == NODE_TYPE_NUM)
            printf ("%f ", *(double *)&node->value);
        else if (node->type == NODE_TYPE_VAR)
            printf ("%c ", *(char *)&node->value);
    }
    else
    {
        switch (*(operation_t *)&node->value)
        {
            case OPERATION_ADD:
                printf ("+ ");
                break;
            case OPERATION_SUB:
                printf ("- ");
                break;
            case OPERATION_MUL:
                printf ("* ");
                break;
            case OPERATION_DIV:
                printf ("/ ");
                break;
            default:
                assert(0 && "no_such operation");
        }
    }

    if (node->right)
        print_tree(node->right);

}

bool is_leaf(Node *node)
{
    assert(node);

    return !node->left || !node->right;
}

void node_dtor (Node *node)
{
    assert(node);

    if (node->right)
    {
        if (!is_leaf(node->right))
            node_dtor(node->right);

        else
            free(node->right);
    }

    if (node->left)
    {
        if (!is_leaf(node->left))
            node_dtor(node->left);

        else
            free(node->left);
    }

    free(node);
    return;
}

