#include "../lib/mtrack.h"

extern int f(int);

int main() {
	int A[10], sum;
	for (int i = 0; i < 10; i++) {
		A[i] = f(i);
		sum += A[i];
	}
	return f(sum);
}
