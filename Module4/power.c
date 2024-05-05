#include <stdio.h>

int power_while(int arg, int factor);
int power_for(int arg, int factor);

int main(void)
{
	printf("%d\n", power_while(2, 10));
	printf("%d\n", power_for(2, 10));
	
	return 0;
}

int power_while(int arg, int factor)
{
	if (factor < 0)
		return -1;
	
	if (0 == factor)
		return 1;
	
	int result = arg;
	while (--factor > 0) {
		result *= arg;
	}
	return result;
}

int power_for(int arg, int factor)
{
	if (factor < 0)
		return -1;
	
	if (0 == factor)
		return 1;
	
	int result = arg;
	for (int i = 1; i < factor; i++) {
		result *= arg;
	}
	
	return result;
}
