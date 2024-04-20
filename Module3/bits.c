#include <stdio.h>

void set_bits(int *value, int *mask)
{
	*value |= *mask;
}

void clear_bits(int *value, int *mask)
{
	*value &= ~*mask;
}

void toggle_bits(int *value, int *mask)
{
	*value ^= *mask;
}

int main(void)
{
	int value = 0b10101, mask = 0b01010;
	
	toggle_bits(&value, &mask);
	
	printf("%d\n", value);
	
	return 0;
}
