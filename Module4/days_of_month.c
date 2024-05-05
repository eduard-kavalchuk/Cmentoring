#include <stdio.h>

void days_in_month(int month);

int main(void)
{
	for (int i = 0; i < 14; i++) {
		days_in_month(i);
	}
	
	return 0;
}

void days_in_month(int month)
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
			printf("%d\n", 28);
			break;
		default:
			printf("Invalid input\n");
			break;
	}
}
