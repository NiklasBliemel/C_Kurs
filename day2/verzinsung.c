#include <stdio.h>
#include <stdlib.h>

int main(){
    double kapital;
    double zielkapital;
    double zins;
    int jahre = 0;

    printf("Startkapital: ");
    scanf("%lf", &kapital);
    printf("Zins: ");
    scanf("%lf", &zins);
    printf("Zielkapital: ");
    scanf("%lf", &zielkapital);

    double diff = zielkapital - kapital;

    while (kapital <= zielkapital)
    {   
        kapital += zins * kapital;
        if (diff > zielkapital - kapital)
        {
            jahre ++;
            diff = zielkapital - kapital;
        }
        else
        {
            printf("Zielkapital nicht erreichbar!\n");
            exit(1);
        }   
    }
    printf("Zielkapital in %d Jahren\n", jahre);
}