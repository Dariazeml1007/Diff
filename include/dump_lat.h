#ifndef LAT_DUMP
#define LAT_DUMP

#include "tree_expression.h"

int dump_to_lat_begin (Node *node, FILE *p_file);
int dump_to_lat_end (Node *node, FILE *p_file);
void print_tree_to_lat (Node *node, FILE *p_file);
void print_explanation(Node *node, FILE *p_file);
void print_node_to_lat(Node *node, FILE *p_file);
void print_dif_node_to_lat (Node *node, FILE *p_file);
#endif
