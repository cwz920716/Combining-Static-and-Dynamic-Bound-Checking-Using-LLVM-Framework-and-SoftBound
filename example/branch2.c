int A[10];

int extra(int x);

int small(int s)
{
	int i = 0, call1, call2;
	call1 = extra(s);
	i = call1;
	if (s < 0) {
		call2 = extra(extra(s));
		i = call2;
	}
	
	return i + call1;
}