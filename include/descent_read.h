#ifndef READ_DES
#define READ_DES

#include "tree_expression.h"
void SyntaxError(int *p);
Node *GetG   (Node *root, int *index);
Node *GetE   (Node *root, int *index);
Node *GetT   (Node *root, int *index);
Node *GetP   (Node *root, int *index);
Node *GetN   (Node *root, int *index);
Node *GetVar (Node *root, int *index);
Node *GetOp  (Node *root, int *index);
Node *GetPow (Node *root, int *index);

#endif
