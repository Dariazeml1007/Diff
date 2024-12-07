#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>

#include "descent_read.h"
#include "dif.h"
#include "tree_expression.h"

Node *GetG(Node *root, int *index)
{
    Node *val = GetE(root, index);
    if (root[*index].value.data.operation != OPERATION_END)
        SyntaxError(index);
    (*index)++;
    return val;
}

Node * GetE(Node *root, int *index)
{
    Node *val = GetT(root, index);
    while (root[*index].value.data.operation == OPERATION_ADD || root[*index].value.data.operation == OPERATION_SUB)
    {
        Node *old = val;
        val = root + *index;

        old->parent = val;
        val->left = old;
        (*index)++;

        val->right = GetT(root, index);
        val->right->parent = val;
    }
    return val;
}

Node * GetT(Node *root, int *index)
{
    Node *val = GetPow(root, index);
    while (root[*index].value.data.operation == OPERATION_MUL || root[*index].value.data.operation == OPERATION_DIV)
    {
        Node *old = val;
        val = root + *index;

        val->left = old;
        val->left->parent = val;
        (*index)++;

        val->right = GetPow(root, index);
        val->right->parent = val;
    }
    return val;
}

Node *GetPow (Node *root, int *index)
{
    Node *val = GetP(root, index);
    while (root[*index].value.data.operation == OPERATION_POWER)
    {
        Node *old = val;
        val = root + *index;

        val->left = old;
        val->left->parent = val;
        (*index)++;

        val->right = GetP(root, index);
        val->right->parent = val;
    }
    return val;
}

Node * GetP (Node *root, int *index)
{
    if (root[*index].value.data.operation == OPERATION_OPEN_BRACKET)
    {
        (*index)++;
        Node * val = GetE(root, index);
        if (root[*index].value.data.operation != OPERATION_CLOSE_BRACKET)
            SyntaxError(index);
        (*index)++;
        return val;
    }

    if (root[*index].value.type == NODE_TYPE_NUM)
        return GetN(root, index);
    if (root[*index].value.type == NODE_TYPE_VAR)
        return GetVar(root, index);
    if (root[*index].value.type == NODE_TYPE_OPER)
        return GetOp(root, index);

    SyntaxError(index);
    return NULL;
}

Node *GetOp (Node *root, int *index)
{

    if (root[*index].value.type != NODE_TYPE_OPER)
        SyntaxError(index);

    Node *res = root + *index;
    (*index)++;

    if (root[*index].value.data.operation == OPERATION_OPEN_BRACKET)
    {
        (*index)++;

        res->right = GetE(root, index);
        res->right->parent= res;

        if (root[*index].value.data.operation != OPERATION_CLOSE_BRACKET)
            SyntaxError(index);
        (*index)++;
        return res;
    }
    SyntaxError(index);
    return NULL;
}

Node * GetVar(Node *root, int *index)
{

    if (root[*index].value.type != NODE_TYPE_VAR)
        SyntaxError(index);
    Node *res = root + *index;
    (*index)++;
    return res;
}



Node * GetN(Node *root, int *index)
{

    if (root[*index].value.type != NODE_TYPE_NUM)
        SyntaxError(index);
    Node *res = root + *index;
    (*index)++;
    return res;
}


void SyntaxError(int *p)
{
    printf ("ERROR %d", *p);
    exit(0);
}
