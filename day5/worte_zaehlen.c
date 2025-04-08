#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("No file given!\n");
        return 1;
    }

    char c;
    char last_c;
    int num_c = 0;
    int num_w = 0;
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Fehler beim Öffnen der Datei.\n");
        return 1;
    }
    while ((c = (char)fgetc(file)) != EOF)
    {
        if (c != '\n')
        {
            num_c++;
        }
        if (c == ' ' && last_c != ' ')
        {
            num_w++;
        }
        last_c = c;
    }
    if (last_c != ' ')
    {
        num_w++;
    }

    printf("%d Zeichen und %d Wörter\n", num_c, num_w);
}