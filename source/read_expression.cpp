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

#define _NUM(a) new_node(NODE_TYPE_NUM, NULL, NULL, a)
#define _x new_node(NODE_TYPE_VAR, NULL, NULL,  'x' )
#define _ADD(a, b) new_node( NODE_TYPE_OPER, a, b, OPERATION_ADD)
#define _SUB(a, b) new_node(NODE_TYPE_OPER, a, b, OPERATION_SUB)
#define _MUL(a, b) new_node(NODE_TYPE_OPER, a, b, OPERATION_MUL)
#define _DIV(a, b) new_node(NODE_TYPE_OPER, a, b, OPERATION_DIV)

//((20)+(x)) (+(20)(x))
Node * read_from_string (Node *node, const char *expression, int *index)
{
    while (expression[*index] != '(' && expression[*index] != ')')
            (*index)++;
    printf ("pointer in expr = %s index = %d\n", expression + (*index), *index);
    if(expression[(*index)++] == '(')
    {
        double number = 0;
        char c = '0';
        if (sscanf(expression + (*index), "%lf", &number) == 1)
        {
            printf ("num = %lf index = %d\n", number, *index);
            Node *new_node_ = _NUM(number);
            if (!new_node_)
                assert(0 && "error null pointer");
            new_node_->parent = node;
            while (expression[*index] != '(' && expression[*index] != ')')
                (*index)++;
            (*index)++;
            printf ("exp = %s index = %d\n", expression + (*index), *index);
            return new_node_;
        }
        else if (sscanf(expression + (*index), "%c", &c) == 1 && c == 'x')
        {
            Node *new_node_ = _x;
            new_node_->parent = node;
            while (expression[*index] != '(' && expression[*index] != ')')
                (*index)++;
            (*index)++;
            return new_node_;
        }

        else if (c == '+')
        {
            Node *left = read_from_string(node, expression, index);
            Node *right = read_from_string(node, expression, index);
            Node *new_node_ = _ADD(left, right);
            return new_node_;
        }



    }
    else if (expression[(*index)++] == ')')
        return node;

    return NULL;
}

#undef _NUM
#undef _x
#undef _ADD
#undef _SUB
#undef _MUL
#undef _DIV
