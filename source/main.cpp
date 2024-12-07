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
#include "dump_lat.h"

int main()
{
    const char *s = "sin(0)+cos(0)+x+sin(x)$";
    Node *root = token(s);
    //print_token(root, strlen(s));
    if (!root)
        assert(0 && "Problem with token\n");

    Node *node = process_str(root);

    draw_and_show(node);

    FILE *p_file = fopen ("latex1.txt", "w");
    if (!p_file)
        assert(0 && "file not opened");

    dump_to_lat_begin(node, p_file);

    Node *dif_result = take_derivative(node, p_file);
    if (!dif_result)
        assert(0 && "NULL pointer\n");

    dif_result = optimize(dif_result);
    draw_and_show(dif_result);
    dump_to_lat_end(dif_result, p_file);

    node_dtor(dif_result);

   node_dtor(node);

    if (fclose(p_file) != 0)
        assert(0 && "not closed");

}


