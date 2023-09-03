/*
 * Author: Aidan Undheim (sorry)
 * Polish notation calculator
 *
 * Description: Polish notation calculator utilizing recursion.
 * Nerdy Description: This calculator works by splitting something like
 * eval(+ exp1 exp2) into eval(exp1) + eval(exp2) recursively. Eventually 
 * it reaches the point where it is evaluating just a number on its own, at
 * which point it just has to turn the string into the actual number 
 * representation.
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
	int isVar;
	int isNum;
};

struct term nextTerm(char *s, size_t start);
struct term findLastArg(char *s, struct term operation);
double evaluate(char *s, size_t start);

static double vars[26];
static double last = 0;

int main()
{
	char s[BUFSIZE];
	printf("~ ");
	while (fgets(s, BUFSIZE, stdin) != NULL) {
		printf("%f\n", (last = evaluate(s, 0)));
		printf("~ ");
	}
	putchar('\n');

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

	ans.isNum = true;
	if (!isdigit(s[start]) && s[start] != '-' && s[start] != '.') {
		ans.isNum = false;
	}

	ans.end = start + 1;
	while (!isspace(s[ans.end]) && s[ans.end] != '\0') {
		if (!isdigit(s[ans.end]) && s[ans.end] != '.' && s[ans.end] != 'e'
		&& s[ans.end] != 'E') {
			ans.isNum = false;
		}

		++ans.end;
	}
	--ans.end;

	// ensure '-' isnt treated as a number
	if (ans.end == ans.start && s[ans.start] == '-') {
		ans.isNum = false;
	}

	// single uppercase letters are variable
	ans.isVar = false;
	if (ans.end == ans.start && 0x41 <= s[ans.start] && s[ans.start] <= 0x5A) {
		ans.isVar = true;
	}

	// special variable
	if (ans.end - ans.start == 2 && strncmp(s + ans.start, "ans", 3) == 0) {
		ans.isVar = true;
	}

	return ans;
}

/*
 * functions by finding two numbers per operation it goes over
 */
struct term findLastArg(char *s, struct term operation)
{
	struct term currentT;
	int numsFound = 0, numsToFind = 2;
	if (operation.isNum || operation.isVar) {
		return operation;
	}

	// its actually an operation, so we need to actually do our job
	currentT = operation;
	while (numsFound < numsToFind) {
		currentT = nextTerm(s, currentT.end + 1);
		if (currentT.isNum || currentT.isVar) {
			++numsFound;
		} else {
			// we only add one. Although we need to find 2 for
			// this operation, the result of this operation also
			// counts towards the total
			++numsToFind;
		}
	}

	return currentT;
}

/*
 * s should, from s[start] to s[end] inclusive, contain a polish notation
*/
double evaluate(char *s, size_t start)
{
	struct term op, term1, term2, lastArg;
	double num1, num2;
	size_t length;

	op = nextTerm(s, start);
	length = op.end - op.start + 1;
	if (op.isNum) {
		return strtod(s + start, NULL);
	}

	if (op.isVar) {
		if (op.start == op.end) {
			return vars[s[op.start] - 0x41];
		} else {
			return last;
		}
	}

	term1 = nextTerm(s, op.end + 1);
	num1 = evaluate(s, term1.start);

	lastArg = findLastArg(s, term1);
	term2 = nextTerm(s, lastArg.end + 1);
	num2 = evaluate(s, term2.start);
	if (strncmp(s + op.start, "+", length) == 0) {
		return num1 + num2;
	} else if (strncmp(s + op.start, "-", length) == 0) {
		return num1 - num2;
	} else if (strncmp(s + op.start, "*", length) == 0) {
		return num1 * num2;
	} else if (strncmp(s + op.start, "/", length) == 0) {
		return num1 / num2;
	} else if (strncmp(s + op.start, "sto", length) == 0) {
		if (!term2.isVar) {
			fputs("Cannot sto into non-variable\n", stderr);
			exit(1);
		}

		if (term2.start == term2.end) {
			vars[(int)(s[term2.start] - 0x41)] = num1;
		} else {
			last = num1;
		}
		return num1;
	} else {
		/* no operation has been matched. We can desecrate s
		 * as we will error out anyway
		*/
		s[op.end + 1] = '\0';
		fprintf(stderr, "Unknown Operation: %s\n", s + op.start);
		exit(1);
	}
}
