#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define BUF_SIZE 2048

// auxiliary.c headers
void help(void);
void quit(void);
void error(void);

// checkInput.c headers
int checkInput (char *);
int isCharInString (char *, char);

// simplifyInput.c headers
void simplifyInput(char *);
void removeChar(char *, char);

// calc.c headers
int calc(char *);