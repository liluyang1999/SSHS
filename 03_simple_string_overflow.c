/*
 * compile: gcc -m32 03_simple_string_overflow.c  -o  out_03_simple_string_overflow
 * run: ./out_03_simple_string_overflow
 * What is the output and why?
 * How would YOU fix it?
 */
#include <stdio.h>
#include <string.h>


int main()
{
        char A[8];
        unsigned short B = 1979;
        printf("Value B: %d\n",B);

        strncpy(A, "excessive", 7);
        printf("Value B: %d\n",B);
        printf("%s", A);
        return 0;
}
