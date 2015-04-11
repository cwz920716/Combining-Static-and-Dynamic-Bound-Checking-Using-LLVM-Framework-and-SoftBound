int A[10];

int sas(int a, int b) {
	int i = (a > b) ? a : b;
	int B[10];
	B[i] = A[i];
	A[i] = 0;
	return A[i];
}