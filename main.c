/*
 * Author: Aidan Undheim (sorry)
 * Polish notation calculator (hopefully)
 *
 * Description: Polish notation calculator utilizing recursion.
 * Nerdy Description: This calculator works by splitting the input into
 * "chunks" of smaller, polish notation calculations. Eventually, when the
 * chunk is reduced to a simple expression, like + 5 2, it will actually
 * evaluate it and return its value. If it were cool, it would also do stuff
 * like remember where all the paranthesis it saw in the last function call
 * were to save itself from going through again, but that would make it less
 * recursion-y
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define BUFSIZE 512

struct term {
	size_t start;
	size_t end;
	int isNum;
}

struct term nextTerm(size_t start);
double evaluate(char *s, struct term t);

int main()
{
	char s[BUFSIZE];
	size_t start = 0;
	size_t end = 0;
	while (s[end] != '\0' && end < BUFSIZE - 1) ++end;
	
	// pray that, by luck, the garbage that is in s happens to form
	// a syntatically correct polish notation expression C-String
	
	// or maybe add a way to get input later

	// I prefer the first option

	
	printf("%f\n", evaluate(s, start, end));
}

/*
 * finds the next parenthesis-contained substring OR
 * finds the next continuous block of non-whitespace chars
 * whichever it finds first
*/
struct term nextTerm(size_t start)
{
	
}

/*
 * s should, from s[start] to s[end] inclusive, contain a polish notation
 * expression with one less pair of beginning and end parenthesis than were
 * there last time.
*/
int evaluate(char *s, struct term t)
{
	
}
