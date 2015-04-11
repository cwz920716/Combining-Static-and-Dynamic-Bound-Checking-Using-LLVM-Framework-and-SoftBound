int array(int *ptr, int len, int c) {
	int a[10];
	int i = 0;
	for (i = 0; i < len; i++)
		ptr[i] = a[i%10] + c;
	return i;
}