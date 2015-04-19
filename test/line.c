int s(int);
int bop(int, int);

int global;

int line() {
	int a = s(1);
	int b = s(2);
	int c = bop(a, b);
	int d = s(c);
	global = a + b;
	return s(d);
}