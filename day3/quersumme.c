#include <stdio.h>

int quersumme(int num)
{
    if (num < 10)
    {
        return num;
    }
    else
    {
        return num % 10 + quersumme(num / 10);
    }  
}

int main()
{
    int N;
    printf("Eingabe N: "); scanf("%d", &N);
    printf("Quersumme = %d\n", quersumme(N));
}
