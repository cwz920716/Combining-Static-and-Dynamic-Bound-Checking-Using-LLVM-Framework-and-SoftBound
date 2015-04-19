int x;

int extra(int *);
int extra2(void *, int);
int f(int);

struct st {
	int a;
	double f;
};

int f(int s) {
  int i = 0, j = 0;
  struct st sts[10];

  if (s > 0) {
    int A[s * 2];
    extra(&A[s]);
    return 0;
  }

  while (i < s + 10) {
    char A[s];
    int y = extra(&sts[i].a);
    while (j < s)
      j = extra2(&A[y], j);
    i = extra(&j);
  }
  return i;
}
