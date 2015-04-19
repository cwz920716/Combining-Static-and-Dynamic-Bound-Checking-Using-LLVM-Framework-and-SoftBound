int f(int);

int sw(int a) {
	int b;
	switch (a) {
		case 0:
			return 1;
		case 1: 
			b = b + 1;
		case 2:
			b = b * 2;
			break;
		case 3:
			b = f(b);
			break;
		default:
			return 0;
	}
	return b;
}