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

int skipping (const char *expression, int *index);

int skipping (const char *expression, int *index)
{
    assert(expression);
    assert(index);

    while (expression[*index] != ')')
            (*index)++;
        (*index)++;

    return 0;
}

Node * read_from_string (Node *node, const char *expression, int *index)
{
    Node *new_node_ = (Node*) calloc (1, sizeof(Node));

    printf ("pointer in expr = %s index = %d\n", expression + (*index), *index);
    if(expression[++(*index)] == '(')
    {
        new_node_->left = read_from_string(new_node_, expression, index);
    }

    double number = 0;

    if (sscanf(expression + (*index), "%lf", &number) == 1)
    {
        new_node_= new_node(NODE_TYPE_NUM, NULL, NULL, number);
        new_node_->parent = node;
        skipping (expression, index);

        printf ("exp = %s index = %d\n", expression + (*index), *index);
        return new_node_;
    }
    else if (expression[*index] == 'x')
    {
        new_node_ = new_node(NODE_TYPE_VAR, NULL, NULL, 'x');
        new_node_->parent = node;
        skipping (expression, index);
        return new_node_;
    }
    else
    {
        new_node_->type = NODE_TYPE_OPER;
        switch (expression[*index])
        {
            case '+':
            {
                *(char *)&new_node_->value = OPERATION_ADD;

                break;
            }
            case '-':
            {
                *(char *)&new_node_->value = OPERATION_SUB;
                break;
            }
            case '*':
            {
                *(char *)&new_node_->value = OPERATION_MUL;
                break;
            }
            case '/':
            {
                *(char *)&new_node_->value = OPERATION_DIV;
                break;
            }
            default:
                assert(0 && "not correct symbol");

        }
        if (expression[++(*index)] == '(')
            new_node_->right = read_from_string(new_node_, expression, index);

        (*index)++;
        printf ("finish_exp = %s index = %d\n", expression + (*index), *index);
        return new_node_;
    }
    return NULL;
}


