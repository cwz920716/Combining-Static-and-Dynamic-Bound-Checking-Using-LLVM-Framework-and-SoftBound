int x;

int extra(int);
int extra2(int, int);

int f(int s, int s1) {
  int i = 0;
  for (i = 0; i < s; i++)
	extra(i);
  return i;
}