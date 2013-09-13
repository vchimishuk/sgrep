#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include "ast.h"
#include "utils.h"


// Returns 1 if given character is repetition operation.
static int is_repetition(char ch)
{
        return ch == '*' || ch == '+';
}

static ast_node *new_node(char ch)
{
        ast_node *node = malloc(sizeof(ast_node));

        bzero(node, sizeof(ast_node));
        node->ch = ch;

        return node;
}

// Create AST node for single character.
static ast_node *create_char_node(char ch)
{
        return new_node(ch);
}

// Create concatenation of two nodes.
// For instance "ab" regexp should be represented as tree-nodes tree:
// concat node and two child char nodes (a and b).
static ast_node *create_concat_node(ast_node *op1, ast_node *op2)
{
        ast_node *node = new_node(AST_OP_CONCAT);

        node->op1 = op1;
        node->op2 = op2;

        return node;
}

// Prepend node with repetition operation node.
static ast_node *create_repetition_node(ast_node *op, char rep)
{
        if (rep == '*') {
                rep = AST_OP_REP_ZERO_OR_MORE;
        } else if (rep == '+') {
                rep = AST_OP_REP_ONE_OR_MORE;
        }

        ast_node *rep_node = new_node(rep);

        rep_node->op1 = op;

        return rep_node;
}

// Join two nodes with alternation operation ('|').
static ast_node *create_alternation_node(ast_node *op1, ast_node *op2)
{
        ast_node *node = new_node(AST_OP_ALTERNATION);
        
        node->op1 = op1;
        node->op2 = op2;

        return node;
}

ast_node *ast_build(char *regexp)
{
        ast_node *tree = NULL;
        char *p = regexp;

        while (*p) {
                ast_node *node;
                char ch = *p;

                if (isalnum(ch)) {
                        node = create_char_node(ch);
                } else if (ch == '|') {
                        node = ast_build(p + 1);
                        tree = create_alternation_node(tree, node);
                        // Rest of the regexp was parsed by ast_buil call,
                        // so there is nothing to do more with this regexp.
                        break;
                } else if (ch == '(') {
                        char *start = ++p;
                        char *end = strchr(start, ')');

                        if (end == NULL) {
                                die("Closing parenthesis not found.");
                        }

                        char *subre = strndup(start, end - start);

                        node = ast_build(subre);
                        p = end;
                        free(subre);
                } else {
                        die("Unexpected character %c.", ch);
                }

                p++;

                if (is_repetition(*p)) {
                        node = create_repetition_node(node, *p);
                        p++;
                }

                if (tree == NULL) {
                        tree = node;
                } else {
                        tree = create_concat_node(tree, node);
                }
        }

        return tree;
}

void ast_free(ast_node *node)
{
        if (node->op1) {
                ast_free(node->op1);
        }
        if (node->op2) {
                ast_free(node->op2);
        }
        free(node);
}

// TEST CODE.
/*
static void print_tree(ast_node *tree)
{
        printf("Node: %p, ch: %d (%c), op1: %p, op2: %p\n",
               tree, tree->ch, tree->ch, tree->op1, tree->op2);

        if (tree->op1 != NULL) {
                print_tree(tree->op1);
        }
        if (tree->op2 != NULL) {
                print_tree(tree->op2);
        }
}

int main()
{
        ast_node *tree = ast_build("a(b|c)d");

        print_tree(tree);

        free_tree(tree);
        
        return 0;
}

*/
