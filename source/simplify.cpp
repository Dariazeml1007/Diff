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
Node *calculate (Node *node);

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

Node *simplifier_constants(Node *node)
{
    assert(node);

    if (is_leaf(node))
        return node;

    if (node->left)
    {
        if (!is_var_in_node(node->left))
        {
            node->left = calculate (node->left);
        }
        else
        {
            node->left = optimize(node->left);
        }
    }
    if (node->right)
    {
        if (!is_var_in_node(node->right))
        {
            node->right = calculate (node->right);
        }
        else
        {
            node->right = optimize(node->right);
        }
    }
    if (node->right && node->left)
        if (!is_var_in_node(node->left) && !is_var_in_node(node->right))
        {
            return calculate(node);
        }

    return node;
}

Node *opt_add (Node *node)
{
    assert (node);

    Node *old_node = node;
    if (node_is_zero(node->left))
    {
        free(old_node);//////dtor
        return copy_subtree(node->right);
    }
    if (node_is_zero(node->right))
    {
        free(old_node);
        return copy_subtree(node->left);
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
        free(old_node);
    }
    else if (node_is_zero(node->left))
    {
        node = NUM(0);
        free(old_node);
    }

    if (node_is_one(node->right))
    {
        node = copy_subtree(node->left);
        free(old_node);
    }
    else if(node_is_zero(node->right))
    {
        node = NUM(0);
        free(old_node);
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
        free(old_node);
    }
    else if (node_is_zero(node->left))
    {
        node = NUM(0);
        free(old_node);
    }

    if (node_is_one(node->right))
    {
        node = copy_subtree(node->left);
        free(old_node);
    }
    else if(node_is_zero(node->right))
    {
        node = NUM(1);
        free(old_node);
    }

    return node;
}

Node *calculate (Node *node)
{
    assert(node);

    Node *old_node = node;

    switch (node->value.type)
    {
        case NODE_TYPE_NUM:
        {
            node = copy_subtree(node);
            break;
        }
        case NODE_TYPE_OPER://func
        {
            switch(node->value.data.operation)
            {
                case OPERATION_ADD:
                {
                    node = NUM((calculate(node->left))->value.data.number + (calculate(node->right))->value.data.number);
                    break;
                }
                case OPERATION_SUB:
                {
                    node = NUM(calculate(node->left)->value.data.number - calculate(node->right)->value.data.number);
                    break;
                }
                case OPERATION_MUL:
                {
                    node = NUM(calculate(node->left)->value.data.number * calculate(node->right)->value.data.number);
                    break;
                }
                case OPERATION_DIV:
                {
                    node = NUM(calculate(node->left)->value.data.number / calculate(node->right)->value.data.number);
                    break;
                }
                case OPERATION_POWER:
                {
                    node = NUM(pow(calculate(node->left)->value.data.number, calculate(node->right)->value.data.number));
                    break;
                }
                case OPERATION_SIN:
                {
                    node = NUM(sin(calculate(node->right)->value.data.number));
                    break;
                }
                case OPERATION_COS:
                {
                    node = NUM(cos(calculate(node->right)->value.data.number));
                    break;
                }
                case OPERATION_LN:
                {
                    node = NUM(log(calculate(node->right)->value.data.number));
                    break;
                }
                case OPERATION_UNKNOWN:
                    assert(0 && "unknown oper");
                default:
                    assert(0 && "wrong oper");
            }
            break;
        }
        case NODE_TYPE_VAR:
            assert(0 && "type var can not be here");
        default:
             assert(0 && "wrong type");
    }
    free(old_node);
    return node;
}


