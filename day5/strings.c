#include <stdio.h>
#include <stdlib.h>

char *extend_string(char *string, int len_string, char c)
{
    string = realloc(string, (len_string + 1) * sizeof(char));
    string[len_string] = c;
    return string;
}

int main(int argc, char **argv)
{
    char *dynamic_string = malloc(0);
    int len_string = 0;
    int j;
    for (size_t i = 1; i < argc; i++)
    {
        j = 0;
        while (argv[i][j] != '\0' || j == 1e6)
        {
            dynamic_string = extend_string(dynamic_string, len_string, argv[i][j]);
            j++;
            len_string++;
        }
        dynamic_string = extend_string(dynamic_string, len_string, ' ');
        len_string++;
    }
    printf("%s\n", dynamic_string);
}