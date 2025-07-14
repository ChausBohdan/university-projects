#include <stdio.h>
#include <limits.h>

extern "C" long long int calc(int, int, char);
extern "C" short K = 0x1017;

int main()
{
    printf("Size of int A: %lu byte\n", sizeof(int));
    printf("Size of int C: %lu byte\n", sizeof(int));
    printf("Size of char E: %lu byte\n", sizeof(char));
    long long int A, C, E;
    printf("Enter numbers:\n");

    while (1) {
        printf("A = ");
        if (scanf_s("%lld", &A) != 1 || A < INT_MIN || A > INT_MAX) {
            printf("Error: Number A must be between %d and %d.\n", INT_MIN, INT_MAX);
        }
        else {
            break;
        }
    }

    while (1) {
        printf("C = ");
        if (scanf_s("%lld", &C) != 1 || C < INT_MIN || C > INT_MAX) {
            printf("Error: Number C must be between %d and %d.\n", INT_MIN, INT_MAX);
        }
        else {
            break;
        }
    }

    while (1) {
        printf("E = ");
        if (scanf_s("%lld", &E) != 1 || E < SCHAR_MIN || E > SCHAR_MAX) {
            printf("Error: Number E must be between %d and %d.\n", SCHAR_MIN, SCHAR_MAX);
        }
        else {
            break;
        }
    }
    long long res = (static_cast<long long>(A) - K + (C / 2) - (E * 8));
    printf("\nA - K + C / 2 - E * 8 = %lld\n", res);
    long long res1 = (static_cast<long long>(calc((int)A, (int)C, (char)E)));
    printf("\nResult of calc is: %lld\n", res1);
    return 0; 
}