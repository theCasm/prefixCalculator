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
double evaluate(char *s, size_t start);

int main()
{
	char s[BUFSIZE];
	size_t end = 0;
	while (s[end] != '\0' && end < BUFSIZE - 1) ++end;
	
	// pray that, by luck, the garbage that is in s happens to form
	// a syntatically correct polish notation expression C-String
	
	// or maybe add a way to get input later

	// I prefer the first option

	/*
	 * struct term test = nextTerm(s, 0);
	 * printf("%s\n%d, %d, %d\n", s, test.start, test.end, test.isNum);
	 * 
	 * printf("%s\n%lf\n", s, evaluate(s, 0));
	*/
	
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
	return ans;
}

/*
 * s should, from s[start] to s[end] inclusive, contain a polish notation
*/
double evaluate(char *s, size_t start)
{
	struct term t = nextTerm(s, start);
	if (t.isNum) {
		return strtod(s + start, NULL);
	}

	if (strncmp(s + t.start, "+", t.end - t.start + 1) == 0) {
		struct term t1 = nextTerm(s, t.end + 1);
		struct term t2 = nextTerm(s, t1.end + 1);

		// do the addition (maybe) (if its x + -x) - O(1) btw
		return 0;
	} else {
		// no operation has been matched. We can desecrate s
		// as we will error out anyway
		s[t.end + 1] = '\0';
		fprintf(stderr, "Unknown Operation: %s\n", s + t.start);
		exit(1);
	}
	return 1;
}
