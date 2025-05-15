#include <stdio.h>

void red(void);
void reset(void);
void green(void);
void cyan(void);

void red(void) {
	printf("\033[1;31m");
}
void green(void) {
	printf("\033[1;32m");
}
void cyan(void) {
	printf("\033[1;36m");
}

void reset() {
	printf("\033[0m");
}