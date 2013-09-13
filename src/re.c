/*
 * Simple NFA regular expressions implementation. First it builds NFA
 * from the regexp pattern, and then testing all its states per pass
 * without any drawbacks.
 *
 * Supported language.
 * c    -- a character
 * *    -- any (zero or more) occurrences
 * +    -- one or more occurrences
 * |    -- alternation. Should be surrounded with parentheses
 * (, ) -- used to define alternation
 *
 * String passes regexp only if the whole string matches, like
 * regexp surrounded with ^ and $.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "ast.h"
#include "utils.h"
#include "re.h"


#define RE_OP_PASS 1
#define RE_OP_MATCH 2
#define RE_OP_SPLIT 4
#define RE_OP_FINAL 8

static state *new_state(int op)
{
        state *s;

        s = malloc(sizeof(state));
        bzero(s, sizeof(state));
        s->op = op;

        return s;
}

// Build states graph for the given AST.
// In end will be saved pointer for the last state in the returning
// piece of graph. Two join graph piece with next piece will be enaugh
// end->out1 = another_graph_part;
// It is guaranteed that returned piece has only one end state (out1),
// and out2 should not be used.
static state *ast_to_nfa(ast_node *ast, state **end)
{
        state *s, *p, *q, *e;

        switch (ast->ch) {
        case AST_OP_CONCAT:
                //
                // o--(a)-->o--(b)-->
                //
                s = ast_to_nfa(ast->op1, &p);
                p->out1 = ast_to_nfa(ast->op2, &e);
                break;
        case AST_OP_ALTERNATION:
                //
                //    ->...-|
                //    |     v
                // o--|     o->
                //    |     ^
                //    ->...-|
                //
                e = new_state(RE_OP_PASS);
                s = new_state(RE_OP_SPLIT);
                s->out1 = ast_to_nfa(ast->op1, &p);
                p->out1 = e;
                s->out2 = ast_to_nfa(ast->op2, &p);
                p->out1 = e;
                break;
        case AST_OP_REP_ZERO_OR_MORE:
                //
                // |-(a)--
                // |     |
                // v  ->o-
                // o--|
                //    ->o->
                //
                e = new_state(RE_OP_PASS);
                s = new_state(RE_OP_SPLIT);
                s->out1 = ast_to_nfa(ast->op1, &p);
                p->out1 = s;
                s->out2 = e;
                break;
        case AST_OP_REP_ONE_OR_MORE:
                //
                // |----------|
                // v          |
                // o--(a)-->o-|
                //            |-->o-->
                //
                q = new_state(RE_OP_SPLIT);
                s = ast_to_nfa(ast->op1, &p);
                p->out1 = q;
                q->out1 = s;
                e = new_state(RE_OP_PASS);
                q->out2 = e;
                break;
        default: // Simple character node.
                //
                // o--(a)-->
                //
                s = new_state(RE_OP_MATCH);
                s->ch = ast->ch;
                e = s;
                break;
        }

        if (end != NULL) {
                *end = e;
        }

        return s;
}

struct re *re_compile(char *regexp)
{
        ast_node *ast = ast_build(regexp);
        state *finit;
        state *nfa;

        // Empty regexp given.
        if (ast == NULL) {
                nfa = new_state(RE_OP_FINAL);
                nfa->finit = 1;
        } else {
                nfa = ast_to_nfa(ast, &finit);

                // Append final state at the end.
                finit->out1 = new_state(RE_OP_FINAL);
                finit->out1->finit = 1;

                ast_free(ast);
        }

        re *r = malloc(sizeof(re));
        r->state = nfa;

        return r;
}

// Move all states throug states which do not require character matching.
// If save_final is not set final states are droped, otherwise they are do
// touched.
static list *pass_not_matches(list *states, int save_final)
{
        list *new_states = EMPTY_LIST;
        state *s;
        
        while (states != EMPTY_LIST) {
                s = (state *) states->data;
                states = list_remove(states, states);

                switch (s->op) {
                case RE_OP_PASS:
                        states = list_prepend(states, s->out1);
                        break;
                case RE_OP_SPLIT:
                        states = list_append(states, s->out1);
                        states = list_append(states, s->out2);
                        break;
                case RE_OP_MATCH:
                        new_states = list_append(new_states, s);
                        break;
                case RE_OP_FINAL:
                        if (save_final) {
                                new_states = list_append(new_states, s);
                        }
                        break;
                default:
                        die("Unsupported operation found.");
                }
        }

        return new_states;
}

int re_exec(re *regexp, char *str)
{
        list *states = list_append(EMPTY_LIST, regexp->state);

        // This is useful when input string is empty and re matches it.
        states = pass_not_matches(states, 1);
        
        while (*str && states != EMPTY_LIST) {
                list *new_states = EMPTY_LIST;
                state *s;

                states = pass_not_matches(states, 0);

                // We have only RE_OP_MATCH states here.
                while (states != EMPTY_LIST) {
                        s = (state *) states->data;
                        states = list_remove(states, states);

                        if (s->ch == *str) {
                                new_states = list_append(new_states, s->out1);
                        }
                }

                states = pass_not_matches(new_states, 1);
                str++;
        }

        int matched = 0;

        // Free list.
        while (states) {
                matched |= ((state *) states->data)->finit;
                states = list_remove(states, states);
        }

        return matched;
}

void re_free(re *regexp)
{
        // TODO: Free states graph.
        free(regexp);
}

// TEST CODE.
/*
int main()
{
        re *regexp = re_compile("(a|b*|c)");
        int matched = re_exec(regexp, "bbbd");

        if (matched) {
                printf("Matched.\n");
        } else {
                printf("Not matched.\n");
        }

        return 0;
}
*/
