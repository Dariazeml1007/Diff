#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>

#include "tree_expression.h"
#include "tree_dif_dump.h"
#include "read_expression.h"
#include "dif.h"

const int MAX_SIZE = 5;

int skip_until_bracket (const char *expression, int *index);
Node *get_operation(Node *node, Node *new_node_, char *oper_str);

int skip_until_bracket (const char *expression, int *index)
{
    assert(expression);
    assert(index);

    while (expression[*index] != ')')
            (*index)++;
        (*index)++;

    return 0;
}

Node *read_from_string (Node *node, const char *expression, int *index)
{

    Node *new_node_ = (Node *) calloc (1, sizeof(Node));
    if (!new_node_)
        return NULL;

    printf ("start in func : pointer in expr = %s index = %d\n", expression + (*index), *index);
    if(expression[++(*index)] == '(')
    {
        new_node_->left = read_from_string(new_node_, expression, index);
        new_node_->left->parent = new_node_;
    }

    double number = 0;

    if (sscanf(expression + (*index), "%lf", &number) == 1)
    {
        new_node_= NUM(number);

        skip_until_bracket (expression, index);

        return new_node_;
    }
    else if (expression[*index] == 'x')
    {
        new_node_ = VAR(x);

        skip_until_bracket (expression, index);

        return new_node_;
    }
    else
    {
        Node_value val = {};

        new_node_->value = val;
        new_node_->value.type = NODE_TYPE_OPER;

        char oper_str[MAX_SIZE] = " ";
        sscanf(expression + (*index), "%[^\n)( ]", oper_str);

        new_node_= get_operation(node, new_node_, oper_str);

        (*index) += strlen(oper_str);
        if (expression[(*index)] == '(')
        {
            new_node_->right = read_from_string(new_node_, expression, index);
            new_node_->right->parent = new_node_;
        }

        (*index)++;
        return new_node_;
    }
    return NULL;
}


Node *get_operation(Node *node, Node *new_node_, char *oper_str)
{
    assert(new_node_);
    assert(oper_str);

    new_node_->value.data.operation = OPERATION_UNKNOWN;

    for (size_t i = 1; i < OPER_ARRAY_SIZE; i++)
    {
        if (strcmp(oper_str, array_of_oper[i].str_operation) == 0)
        {
            new_node_->value.data.operation = array_of_oper[i].operation;
            new_node_->parent = node;
            break;
        }
    }

    if (new_node_->value.data.operation == OPERATION_UNKNOWN)
    {
        printf ("Unknown operation\n");
        return NULL;
    }
    return new_node_;
}
