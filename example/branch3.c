
int f(int, ...);

int br(int s, int s1, int s2) {
  if (s > 0)
    f(s);
  else if (s < 0)
    f(-s, s1);
  else
    f(0);
  return s;
}