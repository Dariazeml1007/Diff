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
#include "simplify.h"
#include "descent_read.h"
#include "tokenizator.h"
void print_token (Node *node, size_t size);

int main()
{
    char *s = "(1+x)*2.4$";
    size_t size = strlen(s);
    Node *root = token(s);
    if (!root)
        printf ("Problem with token\n");
    print_token(root, size);

//     struct Node *root = {};
//
//     const char *s ="(cos(((7)+(5))+((2)*(x))))";
//     int index = 0;
//     Node *result = read_from_string(root, s, &index);
//     if (!result)
//         assert(0 && "NULL pointer\n");
//
//     draw_and_show(result);
//
//      result = optimize(result);
//      draw_and_show(result);
//     Node *dif_result = take_derivative(result);
//     if (!dif_result)
//         assert(0 && "NULL pointer\n");
//
//
//     draw_and_show(dif_result);
//     dif_result = optimize(dif_result);
//
//     draw_and_show(dif_result);
//     node_dtor(result);
//     node_dtor(dif_result);
}


void print_token (Node *node, size_t size)
{
    for (size_t i = 0; i < size; i++)
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
