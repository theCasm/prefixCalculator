/*
 * Author: Aidan Undheim (sorry)
 * Polish notation calculator (hopefully)
 *
 * Description: Polish notation calculator utilizing recursion.
 * Nerdy Description: This calculator works by splitting the input into
 * "chunks" of smaller, polish notation calculations. Eventually, when the
 * chunk is reduced to a simple expression, like + 5 2, it will actually
 * do the thing
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>

#define true 1
#define false 0

#define BUFSIZE 512

struct term {
	size_t start;
	size_t end;
	int isNum;
};

struct term nextTerm(char *s, size_t start);
struct term findLastArg(char *s, struct term operation);
double evaluate(char *s, size_t start);

int main()
{
	char s[BUFSIZE];
	fgets(s, BUFSIZE, stdin);

	printf("%f\n", evaluate(s, 0));
	return 0;
}

/*
 * finds the next continuous block of non-whitespace chars
*/
struct term nextTerm(char *s, size_t start)
{
	struct term ans;
	while (isspace(s[start])) ++start;

	if (s[start] == '\0') {
		fputs("Term Search Overflows string\n", stderr);
		exit(1);
	}
	ans.start = start;

	int isNum = true;
	if (!isdigit(s[start]) && s[start] != '-') {
		isNum = false;
	}

	size_t end = start + 1;
	while (!isspace(s[end]) && s[end] != '\0') {
		if (!isdigit(s[end]) && s[end] != '.' && s[end] != 'e'
		&& s[end] != 'E') {
			isNum = false;
		}

		++end;
	}
	ans.end = end - 1;
	ans.isNum = isNum;

	// ensure '-' isnt treated as a number
	if (ans.end == ans.start && s[ans.start] == '-') {
		ans.isNum = false;
	}

	return ans;
}

/*
 * functions by finding two numbers per operation it goes over
 */
struct term findLastArg(char *s, struct term operation)
{
	if (operation.isNum) {
		return operation;
	}

	// its actually an operation, so we need to actually do our job
	int numsFound = 0, numsToFind = 2;
	struct term t = operation;
	while (numsFound < numsToFind) {
		t = nextTerm(s, t.end + 1);
		if (t.isNum) {
			++numsFound;
		} else {
			// we only add one. Although we need to find 2 for
			// this operation, the result of this operation also
			// counts towards the total
			++numsToFind;
		}
	}

	return t;
}

/*
 * s should, from s[start] to s[end] inclusive, contain a polish notation
*/
double evaluate(char *s, size_t start)
{
	struct term t = nextTerm(s, start);
	size_t length = t.end - t.start + 1;
	if (t.isNum) {
		return strtod(s + start, NULL);
	}

	struct term t1 = nextTerm(s, t.end + 1);
	double n1 = evaluate(s, t1.start);

	struct term last = findLastArg(s, t1);
	struct term t2 = nextTerm(s, last.end + 1);
	double n2 = evaluate(s, t2.start);
	if (strncmp(s + t.start, "+", length) == 0) {
		return n1 + n2;
	} else if (strncmp(s + t.start, "-", length) == 0) {
		return n1 - n2;
	} else if (strncmp(s + t.start, "*", length) == 0) {
		return n1 * n2;
	} else if (strncmp(s + t.start, "/", length) == 0) {
		return n1 / n2;
	} else {
		// no operation has been matched. We can desecrate s
		// as we will error out anyway
		s[t.end + 1] = '\0';
		fprintf(stderr, "Unknown Operation: %s\n", s + t.start);
		exit(1);
	}
	return 1;
}
