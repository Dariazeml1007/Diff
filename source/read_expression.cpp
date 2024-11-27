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

const int MAX_SIZE = 5;

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
    if (!new_node_)
        return NULL;

    printf ("pointer in expr = %s index = %d\n", expression + (*index), *index);
    if(expression[++(*index)] == '(')
    {
        printf ("go to left\n");
        new_node_->left = read_from_string(new_node_, expression, index);
    }
    printf("here\n");
    double number = 0;

    if (sscanf(expression + (*index), "%lf", &number) == 1)
    {
        printf("sscanf num\n");

        Node_value val = {};

        val.data.number = number;
        val.type = NODE_TYPE_NUM;

        new_node_= new_node(NULL, NULL, val);
        printf ("scanf num %lf\n", new_node_->value.data.number);
        new_node_->parent = node;
        skipping (expression, index);

        printf ("scanf num exp = %s index = %d\n", expression + (*index), *index);
        return new_node_;
    }
    else if (expression[*index] == 'x')
    {
        printf ("here1\n");
        Node_value val = {};

        val.data.variable = 'x';

        val.type = NODE_TYPE_VAR;
        printf ("exp = %s index = %d\n", expression + (*index), *index);
        new_node_ = new_node( NULL, NULL, val);
        printf ("var = %c\n", new_node_->value.data.variable);
        new_node_->parent = node;
        skipping (expression, index);
        return new_node_;
    }
    else
    {
        Node_value val = {};

        new_node_->value = val;
        new_node_->value.type = NODE_TYPE_OPER;
        printf ("exp = %s index = %d\n", expression + (*index), *index);
        char oper_str[MAX_SIZE] = " ";
        sscanf(expression + (*index), "%[^\n)( ]", oper_str);
        printf ("string_of_oper = %s\n", oper_str);
        new_node_->value.data.operation = OPERATION_UNKNOWN;
        for (size_t i = 1; i < OPER_ARRAY_SIZE; i++)
        {
            if (strcmp(oper_str, array_of_oper[i].str_operation) == 0)
            {
                new_node_->value.data.operation = array_of_oper[i].operation;

                break;
            }
        }
        printf ("operation %d\n", new_node_->value.data.operation);
        if (new_node_->value.data.operation == OPERATION_UNKNOWN)
        {
            printf ("Unknown operation\n");
            return NULL;
        }
        (*index) += strlen(oper_str);
        if (expression[(*index)] == '(')
            new_node_->right = read_from_string(new_node_, expression, index);

        (*index)++;
        printf ("finish_exp = %s index = %d\n", expression + (*index), *index);
        return new_node_;
    }
    return NULL;
}


