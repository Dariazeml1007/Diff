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


int main()
{
    struct Node *root = {};

    const char *s ="(cos(((7)+(5))+((2)*(x))))";
    int index = 0;
    Node *result = read_from_string(root, s, &index);
    if (!result)
        assert(0 && "NULL pointer\n");

    draw_and_show(result);

     result = optimize(result);
     draw_and_show(result);
    Node *dif_result = take_derivative(result);
    if (!dif_result)
        assert(0 && "NULL pointer\n");


    draw_and_show(dif_result);
    dif_result = optimize(dif_result);

    draw_and_show(dif_result);
    node_dtor(result);
    node_dtor(dif_result);
}


