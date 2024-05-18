#include <stdio.h>

double power_while(int arg, int factor);
double power_for(int arg, int factor);

int main(void)
{
	printf("Power while:\n");
	printf("%d ^ %d = %f\n", 2, 0, power_while(2, 0));
	printf("%d ^ %d = %f\n", 3, -2, power_while(3, -2));
	printf("%d ^ %d = %f\n", 3, 4, power_while(3, 4));
	
	printf("\nPower for:\n");
	printf("%d ^ %d = %f\n", 2, 0, power_for(2, 0));
	printf("%d ^ %d = %f\n", 3, -2, power_for(3, -2));
	printf("%d ^ %d = %f\n", 3, 4, power_for(3, 4));
	
	return 0;
}

double power_while(int arg, int factor)
{
	double result = 1.0;
	double multiplier = factor > 0 ? arg : (factor *= -1, 1.0 / arg);
	
	while (--factor >= 0)
		result *= multiplier;
	
	return result;
}

double power_for(int arg, int factor)
{
	double result = 1.0;
	double multiplier = factor > 0 ? arg : (factor *= -1, 1.0 / arg);
	
	for (int i = 0; i < factor; i++) {
		result *= multiplier;
	}
	
	return result;
}
