int A[10];
int g;

int extra();

int small(int s)
{
	int a = A[0], b = g;
	if (a > b) {
		A[0] = b;
		g = a;
	}
	return a;
}