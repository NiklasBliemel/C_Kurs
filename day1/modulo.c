#include <stdio.h>

int main(){
    int num;
    printf("Ganzzahl Eingeben: ");
    scanf("%d", &num);
    printf("Letzte Ziffer ist %d\n", num % 10);
}