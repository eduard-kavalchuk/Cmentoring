#include <stdio.h>
#include <stdlib.h>

/**
 * Converts Fahrenheits to Celsius on request.
 * If the input string is ill-formed converts only its initial portion.
*/

#define F2C(fahrenheits) ((fahrenheits - 32) * 5 / 9)

int main(int argc, char** argv)
{
	char *endptr, buffer[10];
	double fahrenheits;
	
	printf("Enter temperature in Fahrenheits: ");
	if (fgets(buffer, sizeof(buffer), stdin) == NULL)
		return -1;  /* Unexpected error */

	fahrenheits = strtod(buffer, &endptr);
	if (buffer == endptr) {
		printf("Error: failed to convert. Temperature must be a floating-point number");
		return -1;
	}
	
	printf("%f\n", F2C(fahrenheits));
	
	return 0;
}
