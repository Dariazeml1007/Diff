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

Node *GetG(char *s, int *p)
{
    Node *val = GetE(s, p);
    if (s[*p] != '$')
        SyntaxError(p);
    (*p)++;
    return val;
}

Node * GetE(char *s, int *p)
{
    Node *val = GetT(s, p);
    while (s[*p] == '+' || s[*p] =='-')
    {
        int op = s[*p];
        (*p)++;
        Node * val2 = GetT(s, p);
        if (op == '+')
            val = ADD(val, val2);
        else
            val = SUB(val, val2);
    }
    return val;
}
Node * GetP (char *s, int *p)
{
    if (s[*p] == '(')
    {
        (*p)++;
        Node * val = GetE(s, p);
        if (s[*p] != ')')
            SyntaxError(p);
        (*p)++;
        return val;

    }
    // else if (s[*p] == 'x')
    // {
    //     (*p)++;
    //     return VAR(x);
    // }
    else
        return GetN(s, p);
}

Node * GetN(char *s, int *p)
{
    double val = 0;
    int old_p = *p;

    while ('0' <= s[*p] && s[*p] <= '9')
    {
        val = val * 10 + s[*p] - '0';
        (*p)++;
    }
    if (old_p == *p)
    {
        SyntaxError(p);
    }
    return NUM(val);
}

Node * GetT(char *s, int *p)
{
    Node *val = GetP(s, p);
    while (s[*p] == '*' || s[*p] =='/')
    {
        int op = s[*p];
        (*p)++;
        Node* val2 = GetP(s, p);
        if (op == '*')
            val = MUL(val, val2);
        else
            val = DIV(val, val2);
    }
    return val;
}

void SyntaxError(int *p)
{
    printf ("ERROR %d", *p);
    exit(0);
}
