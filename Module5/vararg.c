#include <stdarg.h>
#include <stdio.h>

double sum(int count, ...);

int main(void)
{
	printf("sum = %f\n", sum(4, 1.0, -2.0, 4.0, -5.1));
	
	return 0;
}

double sum(int count, ...)
{
	va_list ap;
	double sum = 0;
	
	va_start(ap, count);
	for (int i = 0; i < count; i++) {
		sum += va_arg(ap, double);
	}
	va_end(ap);
	
	return sum;
}
