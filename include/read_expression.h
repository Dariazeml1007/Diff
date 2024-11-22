#ifndef DIF_READ
#define DIF_READ

#include "tree_expression.h"
#include "tree_dif_dump.h"
#include "read_expression.h"

Node* read_from_string (Node *node, const char *expression, int *index);

#endif
