#include <stdio.h>
#include <stdlib.h>

int main(){
    int jahr;
    int schaltjahr;
    printf("Geben Sie ein Jahr ein: ");
    scanf("%d", &jahr);
    if (jahr % 4 == 0)
    {
        if (jahr % 100 == 0)
        {
            if (jahr % 400 == 0)
            {
                schaltjahr = 1;
            }
            else
            {
                schaltjahr = 0;
            }
        }
        else
        {
            schaltjahr = 1;
        }
    }
    else
    {
        schaltjahr = 0;
    }
    if (schaltjahr == 1)
    {
        printf("%d ist ein Schalftjahr!\n", jahr);
    }
    else
    {
        printf("%d ist kein Schalftjahr!\n", jahr);
    }
    
}