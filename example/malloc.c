#include <stdlib.h>
#include "../lib/mtrack.h"

int x;

int extra(int *);
int extra2(void *, int);
int f(int);

struct st {
	int a;
	double f;
};

int f(int s) {
	void *ptr = malloc(sizeof(struct st) * s);
	return s;
}

int main(int argc, char **argv) {
	int argc2 = 10;
	int A[10], sum = 0;
	for (int i = 0; i < argc2; i++) {
		A[i] = f(i);
		sum += A[i];
	}
	return f(sum);
}
