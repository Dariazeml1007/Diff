#ifndef READ_DES
#define READ_DES

#include "tree_expression.h"
void SyntaxError(int *p);
Node *GetG(char *s, int *p);
Node *GetE(char *s, int *p);
Node *GetT(char *s, int *p);
Node *GetP(char *s, int *p);
Node *GetN(char *s, int *p);

#endif
