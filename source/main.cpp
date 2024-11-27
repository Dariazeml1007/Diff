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

int main()
{
    //double num = 5;
    struct Node *root = {};
    // printf ("%p\n", root);
    // if (!root)
    //     assert(0 && "NULL pointer\n");
    //struct Node *result = new_node(NODE_TYPE_OPER,(new_node(NODE_TYPE_NUM, NULL, NULL, double(5))), new_node(NODE_TYPE_NUM, NULL, NULL, num), OPERATION_ADD);

    const char *s ="(cos(x))";
    int index = 0;
    Node *result = read_from_string(root, s, &index);
    if (!result)
        assert(0 && "NULL pointer\n");
    //printf ("%c", result->value.variable);
    print_tree(result);

    dump(result, "dump.dot");


//     Node *dif_result = take_derivative(result);
//     if (!dif_result)
//         assert(0 && "NULL pointer\n");
//
//
//     print_tree(dif_result);
//     printf("\n");
//     dump(dif_result, "dump.dot");
    system("dot dump.dot -Tpng -o tree.png");

    node_dtor(result);

    //node_dtor(dif_result);
}

