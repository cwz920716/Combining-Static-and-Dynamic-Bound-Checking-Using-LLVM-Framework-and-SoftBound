typedef int (*FunctPtr)(int, int);

int add(int, int);

int magic(int a, int b, FunctPtr f)
{
	if (b >0)
		return f(a, 0);
	return add(a, 1);
}