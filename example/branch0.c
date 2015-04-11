int A[1000];

int extra(int x);
void error(int x);

int small(int s)
{
	int a = 100 + s;
	int b = 25 * s;
	int c = 0;
	if (a > b) {
		A[a] = 0;
		error(a);
		c = extra(b) + A[b];
	} else
		c= extra(a);

	return c;
}