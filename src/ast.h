#ifndef _AST_H_
#define _AST_H_

// Available operations.
#define AST_OP_CONCAT -1
#define AST_OP_ALTERNATION -2
#define AST_OP_REP_ZERO_OR_MORE -3
#define AST_OP_REP_ONE_OR_MORE -4

// Abstract syntax tree node.
// Node can represents simple character, one operand operation and two operand
// operation. If it is one operand operation node representation only op1
// field is used and op2 is ignored.
typedef struct ast_node {
        char ch;
        struct ast_node *op1;
        struct ast_node *op2;
} ast_node;

// Parse regexp string expression and build AST representation.
ast_node *ast_build(char *regexp);
// Destroy tree and free used memory.
void ast_free(ast_node *node);

#endif
