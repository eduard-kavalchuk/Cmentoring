#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void days_in_month(int month, int year);
bool is_leap_year(int year);

int main(void)
{
	char *endptr, buffer[5];
	int year;
	
	printf("Enter year: ");
	if (fgets(buffer, sizeof(buffer), stdin) == NULL)
		return -1;  /* Unexpected error */

	year = strtol(buffer, &endptr, 10);
	if (buffer == endptr) {
		printf("Year must be an integer number.");
		return -1;
	}
	
	for (int i = 1; i <= 12; i++) {
		days_in_month(i, year);
	}
	
	return 0;
}

void days_in_month(int month, int year)
{
	enum { JAN = 1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC };
	
	switch (month) {
		case JAN:
		case MAR:
		case MAY:
		case JUL:
		case AUG:
		case OCT:
		case DEC: 
			printf("%d\n", 31);
			break;
		case APR:
		case JUN:
		case SEP:
		case NOV:
			printf("%d\n", 30);
			break;
		case FEB:
			if (is_leap_year(year))
				printf("%d\n", 29);
			else
				printf("%d\n", 28);
			break;
		default:
			printf("Invalid input\n");
			break;
	}
}

bool is_leap_year(int year)
{
	return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
}
