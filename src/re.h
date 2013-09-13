#ifndef _RE_H_
#define _RE_H_

// Machine's state representation, -- graph's vertex.
// Each state can has maximum two outgoing edges: out1 & out2.
typedef struct state {
        int finit;
        int op;
        char ch;
        struct state *out1;
        struct state *out2; // Only used when op == RE_OP_SPLIT.
} state;

// re is a result object of regexp compiling process, it includes states graph
// build from the regexp (the NFA) and can hold some information with matching
// results.
typedef struct re {
        state *state;
} re;

// Compile regular expression string and return
// result object which can be matched with string
// with re_exec function.
struct re *re_compile(char *regexp);
// Test compiled regular expression with string object.
int re_exec(re *regexp, char *str);
// Destroy re object.
void re_free(re *regexp);

#endif
