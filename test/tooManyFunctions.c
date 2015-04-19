int factor (int n)
{
  if (n == 0)
    return 1;
  else
    return n * factor(n-1);
}

int recursive2(int a);

int recursive1(int a)
{
  if (a % 2 == 0)
    return recursive2(a);
  else
    return a + recursive2(a - 1);
}

int recursive2(int a)
{
  if (a % 2 != 0)
    return recursive1(a);
  else
    return a + recursive1(a - 1);
}

int start(int (*fp) (int, int), int a, int b)
{
  return (*fp)(a, b);
}

int add(int a, int b)
{
  return a + b;
}

int sub(int a, int b)
{
  return a - b;
}

int main()
{
	int (*magic) (int, int);
	factor(10);
	recursive1(15);
	magic = add;
	start(magic, 1, 2);
	return 0;
}