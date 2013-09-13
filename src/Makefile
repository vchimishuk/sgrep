all: sgrep

test: sgrep
	sh test.sh > /dev/null

sgrep:
	gcc -std=gnu99 -Wall -o sgrep sgrep.c utils.c list.c ast.c re.c

clean:
	rm -f *.out sgrep
