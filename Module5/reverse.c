#include <stdio.h>
#include <string.h>

void reverse(char str[]);
void reverse_string(char str[], int start, int len);

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("Provide exactly one agrument.");
		return 1;
	}
	
	reverse(argv[1]);
	printf(argv[1]);
	
	return 0;
}

void reverse(char str[])
{
	reverse_string(str, 0, strlen(str) - 1);
}

void reverse_string(char str[], int start, int finish)
{
	char c;
	
	if (start >= finish)
		return;
	
	c = str[start];
	str[start] = str[finish];
	str[finish] = c;
	
	reverse_string(str, start + 1, finish - 1);
}
