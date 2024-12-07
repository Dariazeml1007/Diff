#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>
#include <ctype.h>
#include <math.h>

#include "tokenizator.h"
#include "tree_expression.h"

const size_t SIZE_OF_OPER = 16;

operation_t get_op (char *oper_str);
double get_num (const char *str, int *index);

Node *token (const char *str)
{
    assert(str);

    int index = 0;
    int jndex = 0;
    size_t size = strlen(str);

    Node *array = (Node *)calloc(size, sizeof(Node));
    if (!array)
        return NULL;

    while (str[index]!= '$')
    {
        if (isdigit(str[index]))
        {
            array[jndex].value.data.number = get_num(str, &index);
            array[jndex++].value.type = NODE_TYPE_NUM;
        }
        else if (isalpha(str[index]))
        {
            char oper_str[SIZE_OF_OPER] = "";
            int kndex = 0;
            while (isalpha(str[index]))
            {
                oper_str[kndex++] = str[index];
                (index)++;
            }
            oper_str[kndex] = '\0';
            operation_t oper = get_op(oper_str);
            if (oper != OPERATION_UNKNOWN)
            {
                array[jndex].value.data.operation = oper;
                array[jndex++].value.type = NODE_TYPE_OPER;
            }
            else
            {
                strcpy(array[jndex].value.data.variable, oper_str);
                array[jndex++].value.type = NODE_TYPE_VAR;
            }
        }
        else
        {
            char symbol_oper[2] = {str[index++], '\0'};
            operation_t oper = get_op(symbol_oper);
            if (oper != OPERATION_UNKNOWN)
            {
                array[jndex].value.data.operation = oper;
                array[jndex++].value.type = NODE_TYPE_OPER;
            }
            else
            {
                assert(0 && "ERROR, not correct oper");
            }
        }

    }
    array[jndex].value.data.operation = OPERATION_END;
    array[jndex++].value.type = NODE_TYPE_OPER;

    return array;
}

operation_t get_op (char *oper_str)
{
    assert(oper_str);

    operation_t name_of_oper = OPERATION_UNKNOWN;
    for (size_t i = 1; i < OPER_ARRAY_SIZE; i++)
    {
        if (strcmp(oper_str, array_of_oper[i].str_operation) == 0)
        {
            name_of_oper = array_of_oper[i].operation;
            break;
        }
    }
    return name_of_oper;
}

double get_num (const char *str, int *index)
{
    assert(str);
    assert(index);

    double num = 0;

    while (isdigit(str[*index]))
    {
        num += num*10 + str[*index] -'0';
        (*index)++;

    }
    if (str[*index] == '.')
    {
        (*index)++;
        int k = 1;
        while (isdigit(str[*index]))
        {
            num += (str[*index] -'0')/(pow(10,k));
            (*index)++;
        }
    }
    return num;
}

void print_token (Node *node, size_t size)
{
    for (size_t i = 0; i < size - 1; i++)
    {
        if (node[i].value.type == NODE_TYPE_NUM)
            printf ("%lf", node[i].value.data.number);
        else if (node[i].value.type == NODE_TYPE_VAR)
            printf("%s", node[i].value.data.variable);
        else
        {
            for (size_t j = 1; j < OPER_ARRAY_SIZE; j++)
            {
                if (node[i].value.data.operation == array_of_oper[j].operation)
                {
                    printf("%s", array_of_oper[j].str_operation);
                }
            }
        }
    }
}
