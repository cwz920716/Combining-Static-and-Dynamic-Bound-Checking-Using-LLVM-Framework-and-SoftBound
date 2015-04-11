int x;

int extra(int);
int extra2(int, int);

int f(int s, int s1) {
  int i = 0;
  for (i = 0; i < s; i++) {
	if (i % 2 == 0)
		continue;
	s1 = extra(i);
	if (s1 > 0)
		break;
  }
  return i;
}