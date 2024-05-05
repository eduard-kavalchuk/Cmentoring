#include <stdio.h>
#include <stdbool.h>

bool isupper_(char c);
bool islower_(char c);

int main(void)
{
    printf("%d\n", isupper_(12));
	printf("%d\n", isupper_('A'));
	printf("%d\n", islower_('B'));
	printf("%d\n", islower_('b'));
	
	return 0;
}

bool islower_(char c)
{
	if (c >= 97 && c <= 122)
		return true;
	return false;
}

bool isupper_(char c)
{
	if (c >= 65 && c <= 90)
		return true;
	return false;
}
