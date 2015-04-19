int ex(int);
void f();

int returnX(int a, int b) {
	int x = a + b;
	int y = x + a;
	if (a > 0) 
		return ex(y);
	x = ex(x);
	if (x > 0)
		return ex(0);
	return ex(x);
}