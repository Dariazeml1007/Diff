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

enum operation_t
{
    OPERATION_UNKNOWN = 0,
    OPERATION_ADD = 1,
    OPERATION_SUB = 2,
    OPERATION_MUL = 3,
    OPERATION_DIV = 4,
    OPERATION_SIN = 5,
    OPERATION_COS = 6,
    OPERATION_POWER = 7,
    OPERATION_LN = 8
};

struct Node_value
{
    Info_t type;
    union
    {
        double number;
        operation_t operation;
        char variable;

    }data;
};

struct Node
{

    Node *left;
    Node *right;
    Node *parent;
    Node_value value;
};

struct oper_prototype
{
    operation_t operation;
    const char *str_operation;
};

static const oper_prototype array_of_oper[] =
{
    {OPERATION_UNKNOWN, ""/*EMPTY SPACE FOR ERRORS CHECKING*/},
    {OPERATION_ADD, "+"},
    {OPERATION_SUB, "-"},
    {OPERATION_MUL, "*"},
    {OPERATION_DIV, "/"},
    {OPERATION_SIN, "sin"},
    {OPERATION_COS, "cos"},
    {OPERATION_POWER, "^"},
    {OPERATION_LN, "ln"}
};
static const size_t OPER_ARRAY_SIZE = sizeof(array_of_oper)/sizeof(array_of_oper[0]);

Node * new_node (Node *left, Node *right, Node_value val);
void print_tree (Node *node);
bool is_leaf(Node *node);
void node_dtor (Node *node);
void draw_and_show (Node *node);


#endif
