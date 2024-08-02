#include <stdio.h>
#include <string.h>

void find_max_min_word(char *input_str, char *max_str, char *min_str);

int main()
{
    char *input = "It is a string with smallest and largest word";
    char max_str[strlen(input) + 1], min_str[strlen(input) + 1];
    
    find_max_min_word(input, max_str, min_str);
    
    printf("Minimum string: %s\n", min_str);
    printf("Maximum string: %s\n", max_str);

	return 0;
}

void find_max_min_word(char *input_str, char *max_str, char *min_str)
{
    char *delimiters = " ";
	char *token;
    char input[strlen(input_str) + 1]; // this is going to be the first argument of strtok, which must be an array
    size_t min_len = strlen(input_str) + 1, max_len = 0;

    strcpy(input, input_str);  // fill *input* array with a content of *input_str* string
    token = strtok(input, delimiters);
    
	while (token) {
        size_t len = strlen(token);
        if(len < min_len) {
            strcpy(min_str, token);
            min_len = len;
        }
        if (len > max_len) {
            strcpy(max_str, token);
            max_len = len;
        }
            
		token = strtok(NULL, delimiters);
	}
}
