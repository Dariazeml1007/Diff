#ifndef OPT
#define OPT

#include "tree_expression.h"
#include "tree_dif_dump.h"
#include "read_expression.h"
#include "dif.h"

#define left_type node->left->value.type
#define left_val node->left->value.data.number
#define right_type node->right->value.type
#define right_val node->right->value.data.number

Node * optimize (Node *node);

#endif
