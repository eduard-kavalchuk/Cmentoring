#include <stdio.h>
#include <stdbool.h>

bool is_odd(int value)
{
	return value & 0b1;
}

bool is_even(int value)
{
	return !(value & 0b1);
}

int main(void)
{
	for (int i = 1; i < 10; i++)
		printf("%d -> %d\n", i, is_even(i));
	
	return 0;
}
