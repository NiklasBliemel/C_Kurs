#include <stdio.h>
#include <stdlib.h>

int main(){
    double a,b;
    char c;
    printf("a c(+-*/) b:\n\n");
    printf("Eingabe a: ");
    scanf("%lf", &a);
    printf("Eingabe c: ");
    scanf(" %c", &c);
    printf("Eingabe b: ");
    scanf("%lf", &b);
    double solution;
    switch (c)
    {
    case '+':
        solution = a + b;
        break;
    case '/':
        if (b != 0){
            solution = a / b;
        }
        else {
            printf("DivisionError: Don't devide with 0");
        }
        break;
    case '*':
        solution = a * b;
        break;
    case '-':
        solution = a - b;
        break;
    default:
        char solution = "C";
    }
    if (solution == "C")
    {
        printf("\"%c\" is a illigal sing!\n", c);
    }
    else 
    {
        printf("%lf %c %lf = %f\n", a, c, b, solution);
    }   
}