#ifndef DIF
#define DIF

#include "tree_expression.h"
#include "read_expression.h"

#define NUM(a) new_node(NULL, NULL, (struct Node_value){.type = NODE_TYPE_NUM, .data = {.number = a}})
#define ADD(a, b) new_node(a, b, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_ADD}})
#define SUB(a, b) new_node(a, b, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_SUB}})
#define MUL(a, b) new_node(a, b, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_MUL}})
#define DIV(a, b) new_node(a, b, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_DIV}})
#define POW(a, b) new_node(a, b, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_POWER}})
#define SIN(a) new_node(NULL, a, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_SIN}})
#define COS(a) new_node(NULL, a, (struct Node_value){.type = NODE_TYPE_OPER, .data = {.operation = OPERATION_COS}})
//#define VAR(x) new_node(NULL, NULL, (struct Node_value){.type = NODE_TYPE_VAR, .data = {.variable = "x"}})


#define dL take_derivative(node->left,  p_file)//p_file
#define dR take_derivative(node->right, p_file)
#define cL copy_subtree(node->left)
#define cR copy_subtree(node->right)

Node *take_derivative(Node *node, FILE*p_file);
bool is_var_in_node (Node *node);
Node* copy_subtree(Node *node);
Node *process_str (Node *root);

#endif

