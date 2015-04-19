#include <setjmp.h>

typedef int (*FunctPtr)(int, int);

int add(int, int);

int magic(int a, int b, FunctPtr f)
{
	jmp_buf env;
	if (setjmp(env) == 1)
		return 0;
	longjmp(env, 1);
	return 1;
}