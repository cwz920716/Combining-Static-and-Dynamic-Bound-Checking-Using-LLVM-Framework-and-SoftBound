typedef int (*FunctPtr)(int, int);

int add(int, int);

int magic(int a, int b, FunctPtr f)
{
	if (b > 0)
		f(a, a);
	else if (b == 0)
		add(0, 0);
	return f(b, b);
}