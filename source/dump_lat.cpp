#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>


#include "dump_lat.h"
#include "tree_expression.h"

int dump_to_lat_begin (Node *node, FILE *p_file)
{
    assert(node);
    assert(p_file);

    fprintf (p_file, "\\documentclass{article}\n"
                            "\\usepackage{graphicx}\n"
                            "\\usepackage[utf8]{inputenc}\n"
                            "\\usepackage[T2A]{fontenc}\n"
                            "\\usepackage[russian]{babel}\n"
                            "\\usepackage{amsmath}\n"
                            "\\usepackage{amssymb}\n"
                            "\\title{Взятие производной}\n"
                            "\\author{Daria Zemlianaia}\n"
                            "\\date{December 2024}\n"
                            "\\begin{document}\n"
                            "\\maketitle\n"
                            "\\section{Пример}\n");

    fprintf (p_file, "Сегодня разберем ооооочень тяжелую задачу : $$");
    print_tree_to_lat(node, p_file);
    fprintf (p_file, "$$\nРазобьем наш пример на части и продифференцируем\n");

    return DIF_SUCCESS;

}
void print_node_to_lat(Node *node, FILE *p_file)
{
    assert(node);
    assert(p_file);

    print_explanation (node, p_file);

    fprintf (p_file, "$$f(x) = ");
    print_tree_to_lat(node, p_file);
    fprintf (p_file, "$$\n");
}
void print_dif_node_to_lat (Node *node, FILE *p_file)
{
    assert(node);
    assert(p_file);

    fprintf (p_file, "Получим :\n$$f'(x) = ");
    print_tree_to_lat(node, p_file);
    fprintf (p_file, "$$\n");
}
void print_explanation(Node *node, FILE *p_file)
{
    assert(node);
    assert(p_file);

    switch (node->value.type)
    {
        case NODE_TYPE_NUM:
        {
            fprintf(p_file, "\nВозьмем производную от константы\n");
            break;
        }
        case NODE_TYPE_VAR:
        {
            fprintf(p_file, "\nВозьмем производную от переменной\n");
            break;
        }
        case NODE_TYPE_OPER:
        {
            switch (node->value.data.operation)
            {
                case OPERATION_ADD:
                {
                    fprintf(p_file, "\nВозьмем производную от суммы\n");
                    break;
                }
                case OPERATION_SUB:
                {
                    fprintf(p_file, "\nВозьмем производную от разности\n");
                    break;
                }
                case OPERATION_MUL:
                {
                    fprintf(p_file, "\nВозьмем производную от умножения\n");
                    break;
                }
                case OPERATION_DIV:
                {
                    fprintf(p_file, "\nВозьмем производную от частного\n");
                    break;
                }
                case OPERATION_POWER:
                {
                    fprintf(p_file, "\nВозьмем производную от степенной функции\n");
                    break;
                }
                case OPERATION_SIN:
                {
                    fprintf(p_file, "\nВозьмем производную от синуса\n");
                    break;
                }
                case OPERATION_COS:
                {
                    fprintf(p_file, "\nВозьмем производную от косинуса\n");
                    break;

                }
                case OPERATION_LN:
                {
                    fprintf(p_file, "\nВозьмем производную от натурального логарифма\n");
                    break;
                }
                case OPERATION_OPEN_BRACKET:
                case OPERATION_CLOSE_BRACKET:
                case OPERATION_END:
                case OPERATION_UNKNOWN :
                default :
                    assert(0 && "not correct operation");

            }
            break;
        }

        default :
            assert(0 && "not correct type");

    }
}
int dump_to_lat_end (Node *node, FILE *p_file)
{

    assert(node);
    assert(p_file);
    fprintf (p_file, "Такой итог : \n$$f'(x) = ");
    print_tree_to_lat(node, p_file);
    fprintf (p_file, "$$\n\\end{document}");

    return DIF_SUCCESS;
}

void print_tree_to_lat (Node *node, FILE *p_file)
{

    if (node == NULL)
    {
        printf ("NULL pointer\n");
        return;
    }

    if (node->left)
    {
        print_tree_to_lat(node->left, p_file);
    }

    if (is_leaf(node))
    {
        if (node->value.type == NODE_TYPE_NUM)
        {

            if (node->parent && node->parent->value.data.operation == OPERATION_POWER && node->parent->right == node )
                fprintf (p_file, "{%lf}", node->value.data.number);
            else if (node->value.data.number < 0)
                fprintf (p_file, "(%lf)", node->value.data.number);
            else
                fprintf (p_file, "%lf", node->value.data.number);
        }
        else if (node->value.type == NODE_TYPE_VAR)
            fprintf (p_file, "%s", node->value.data.variable);
    }
    else
    {
        fprintf (p_file, "%s", array_of_oper[node->value.data.operation].str_operation);
        if (node->value.data.operation == OPERATION_SIN || node->value.data.operation == OPERATION_COS ||
            node->value.data.operation == OPERATION_DIV || node->value.data.operation == OPERATION_LN
        )
            fprintf (p_file, "(");
    }

    if (node->right)
    {
        print_tree_to_lat(node->right, p_file);
        if (node->value.data.operation == OPERATION_SIN || node->value.data.operation == OPERATION_COS ||
            node->value.data.operation == OPERATION_DIV || node->value.data.operation == OPERATION_LN)
            fprintf (p_file, ")");

    }

}
