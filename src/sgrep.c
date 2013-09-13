#include <stdio.h>
#include <stdlib.h>
#include "re.h"
#include "utils.h"


int print_usage()
{
        fprintf(stderr, "Usage: sgrep PATTERN [FILE]...\n");

        return 2;
}

int match_file(FILE *f, re *regexp)
{
        ssize_t read;
        size_t len;
        char *line = NULL;
        int matched = 0;

        while ((read = getline(&line, &len, f)) != -1) {
                if (line[read - 1] == '\n') {
                        line[read - 1] = '\0';
                }
                
                if (re_exec(regexp, line)) {
                        matched |= 1;
                                
                        fwrite(line, sizeof(char), read - 1, stdout);
                        fwrite("\n", sizeof(char), 1, stdout);
                }
                
                free(line);
                line = NULL;
        }
        
        return matched;
}

// Reads input char sequence line by line and test it against the
// regular expression. If line is matched with regexp it is printed
// to stdout.
// Input files usage: sgrep REGEXP FILE1 FILE2
// stdin usage: echo foo | sgrep REGEXP
// If at lease one matching found exit code is 0, otherwise it is 1.
int main(int argc, char *argv[])
{
        if (argc < 2) {
                return print_usage();
        }

        re *regexp;
        int matched;

        regexp = re_compile(argv[1]);

        if (argc > 2) { // Use input files.
                for (int i = 2; i < argc; i++) {
                        FILE *f = fopen(argv[i], "r");

                        if (f == NULL) {
                                die("Failed to open file %s", argv[i]);
                        }

                        matched |= match_file(f, regexp);
                        fclose(f);
                }
        } else { // Use stdin as input.
                matched = match_file(stdin, regexp);
        }

        re_free(regexp);

        return !matched;
}
