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

Node *new_node (Node *left, Node *right, Node_value val)
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
    {
        print_tree(node->left);
    }

    if (is_leaf(node))
    {
        if (node->value.type == NODE_TYPE_NUM)
        {

            if (node->value.data.number < 0)
                printf ("(%lf)", node->value.data.number);
            else
                printf ("%lf", node->value.data.number);
        }
        else if (node->value.type == NODE_TYPE_VAR)
            printf ("%s", node->value.data.variable);
    }
    else
    {
        printf ("%s", array_of_oper[node->value.data.operation].str_operation);
        if (node->value.data.operation == OPERATION_SIN || node->value.data.operation == OPERATION_COS ||
            node->value.data.operation == OPERATION_DIV || node->value.data.operation == OPERATION_LN
        )
            printf ("(");
    }

    if (node->right)
    {
        print_tree(node->right);
        if (node->value.data.operation == OPERATION_SIN || node->value.data.operation == OPERATION_COS ||
            node->value.data.operation == OPERATION_DIV || node->value.data.operation == OPERATION_LN)
            printf (")");

    }

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

void draw_and_show (Node *node)
{
    assert(node);

    printf("the tree :");
    print_tree(node);
    printf("\n");

    dump(node, "dump.dot");
    system("dot dump.dot -Tpng -o tree.png");

}
