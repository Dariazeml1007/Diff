#ifndef DIF_TREE
#define DIF_TREE

#include <stdint.h>



typedef enum
{
    DIF_SUCCESS = 0,
    DIF_ALLOCATION_MEMORY_ERROR = 1,
    DIF_FILE_NOT_OPENED = 2,
    DIF_FILE_NOT_CLOSED = 3,
    DIF_STAT_ERROR = 4,
    DIF_NO_FREE_ELEM = 5,
    NO_SUCH_ITEM_ERROR = 6,
    DIF_READ_ERROR = 7,
    DIF_INSERT_ERROR = 8

}Dif_errors;

enum Info_t
{
    NODE_TYPE_NUM = 1,
    NODE_TYPE_VAR = 2,
    NODE_TYPE_OPER = 3

};

struct Node
{
    Info_t type;
    uint64_t value;
    Node *left;
    Node *right;
    Node *parent;
};

enum operation_t
{
    OPERATION_ADD = 0,
    OPERATION_SUB = 1,
    OPERATION_MUL = 2,
    OPERATION_DIV = 3
};

Node *new_node (Info_t type, Node *right, Node *left, ...);
void print_tree (Node *node);
bool is_leaf(Node *node);
void node_dtor (Node *node);


#endif
