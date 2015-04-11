int A[10];

int main() {
  int i = 9, sum;
  int B[10];

  do {
    if (A[i] < 0)
      A[i] = B[i] + 1;
    else
      A[0] = 1;
    sum += B[i];
    i = i - 1;
  } while ( i >= 0);

  return A[3];
}