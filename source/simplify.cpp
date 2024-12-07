#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>
#include <stdarg.h>
#include <math.h>

#include <simplify.h>
#include <dif.h>

const double EPS = 1e-7;

Node *opt_add (Node *node);
Node *opt_mul (Node *node);
Node *opt_pow (Node *node);
Node *simplifier_operations(Node *node);
Node *simplifier_constants(Node *node);
double calculate (Node *node);
double oper_calc(Node *node);

bool is_zero (double a);
bool node_is_one (Node *node);
bool node_is_zero (Node *node);
bool is_one (double number);

bool is_zero (double number)
{
    return (fabs (number) < EPS);
}
bool is_one (double number)
{
    return (fabs(number - 1) < EPS);
}
bool node_is_zero (Node *node)
{
    return (node) && node->value.type == NODE_TYPE_NUM && is_zero(node->value.data.number);
}
bool node_is_one (Node *node)
{
    return (node) && node->value.type == NODE_TYPE_NUM && is_one(node->value.data.number);
}

Node *optimize (Node *node)
{
    assert(node);

    node = simplifier_constants(node);

    node = simplifier_operations(node);

    return node;
}

Node *simplifier_constants(Node *node)
{
    assert(node);


    if (is_leaf(node))
        return node;

    if (node->left)
    {
        if (!is_var_in_node(node->left))
        {
            Node *old_left = node->left;
            node->left = NUM(calculate (node->left));
            node->left->parent = node;
            node_dtor(old_left);
        }
        else
        {
            node->left = optimize(node->left);
            node->left->parent = node;
        }
    }
    if (node->right)
    {
        if (!is_var_in_node(node->right))
        {
            Node *old_right = node->right;
            node->right = NUM(calculate (node->right));

            node->right->parent = node;
            node_dtor(old_right);
        }
        else
        {
            node->right = optimize(node->right);
            node->right->parent = node;
        }
    }
    if (node->right && node->left)
        if (!is_var_in_node(node->left) && !is_var_in_node(node->right))
        {
            Node *old_node = node;
            node = NUM(calculate(node));
            node_dtor(old_node);
        }

    return node;
}

Node *simplifier_operations(Node *node)
{
    assert(node);

    if (is_leaf(node))
        return node;

    if (node->right)
        node->right = simplifier_operations(node->right);

    if (node->left)
        node->left = simplifier_operations(node->left);

    if (node->value.data.operation == OPERATION_ADD)
    {
        return opt_add(node);
    }
    else if (node->value.data.operation == OPERATION_MUL)
    {
        node = opt_mul(node);
    }
    else if (node->value.data.operation == OPERATION_POWER)
    {
        node = opt_pow(node);
    }

    return node;
}

Node *opt_add (Node *node)
{
    assert (node);
    Node *old_node = node;

    if (node_is_zero(node->left))
    {
        node = copy_subtree(node->right);
        node_dtor(old_node);
    }
    if (node_is_zero(node->right))
    {
        node = copy_subtree(node->left);
        node_dtor(old_node);
    }

    return node;
}


Node *opt_mul (Node *node)
{
    assert (node);
    Node *old_node = node;

    if (node_is_one(node->left))
    {
        node = copy_subtree(node->right);
        node_dtor(old_node);
    }
    else if (node_is_zero(node->left))
    {
        node = NUM(0);
        node_dtor(old_node);
    }
    Node *old_node_ = node;
    if (node_is_one(node->right))
    {
        node = copy_subtree(node->left);
        node_dtor(old_node_);
    }
    else if(node_is_zero(node->right))
    {
        node = NUM(0);
        node_dtor(old_node_);
    }

    return node;
}

Node *opt_pow (Node *node)
{
    assert(node);

    Node *old_node = node;

    if (node_is_one(node->left))
    {
        node = copy_subtree(node->left);
        node_dtor(old_node);
    }
    else if (node_is_zero(node->left))
    {
        node = NUM(0);
        node_dtor(old_node);
    }
    Node *old_node_ = node;
    if (node_is_one(node->right))
    {
        node = copy_subtree(node->left);
        node_dtor(old_node_);
    }
    else if(node_is_zero(node->right))
    {
        node = NUM(1);
        node_dtor(old_node_);
    }

    return node;
}

double calculate (Node *node)
{
    assert(node);

    switch (node->value.type)
    {
        case NODE_TYPE_NUM:
        {
            return node->value.data.number;
        }
        case NODE_TYPE_OPER:
        {
            return oper_calc(node);
        }
        case NODE_TYPE_VAR:
            assert(0 && "type var can not be here");
        default:
            assert(0 && "wrong type");
    }

    return NAN;
}


double oper_calc(Node *node)
{
    assert(node);

    switch(node->value.data.operation)
    {
        case OPERATION_ADD:
        {
            return calculate(node->left) + calculate(node->right);
        }
        case OPERATION_SUB:
        {
            return calculate(node->left) - calculate(node->right);
        }
        case OPERATION_MUL:
        {
            return calculate(node->left) * calculate(node->right);
        }
        case OPERATION_DIV:
        {
            return calculate(node->left)/ calculate(node->right);
        }
        case OPERATION_POWER:
        {
           return pow(calculate(node->left), calculate(node->right));
        }
        case OPERATION_SIN:
        {
            return sin(calculate(node->right));
        }
        case OPERATION_COS:
        {
            return cos(calculate(node->right));
        }
        case OPERATION_LN:
        {
            return log(calculate(node->right));
        }
        case OPERATION_OPEN_BRACKET:
        case OPERATION_CLOSE_BRACKET:
        case OPERATION_END:
        case OPERATION_UNKNOWN:
            assert(0 && "unknown oper");
        default:
            assert(0 && "wrong oper");
    }

    return NAN;
}
