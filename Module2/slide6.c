#include <stdio.h>

int main(void)
{
	int i = 5;
	long int li = 5L;
	long long int lli = 5LL;
	double d = 5.0;
	float f = 5.0F;
	long double ld = 5.0L;
	
	// C standard 6.4.4.4.10: "An integer character constant has type int".
	// char c = 'x' converts integer value 120 (ASCII code for char 'x', 4 bytes) to char (1 byte)
	int c = 'x';
	
	printf("value %zu\n", sizeof(i));
	printf("value %zu\n", sizeof(li));
	printf("value %zu\n", sizeof(lli));
	printf("value %zu\n", sizeof(d));
	printf("value %zu\n", sizeof(f));
	printf("value %zu\n", sizeof(ld));
	printf("value %zu\n", sizeof(c));   // sizeof(char) == 1, sizeof('x') == sizeof(int)
	
	return 0;
}

