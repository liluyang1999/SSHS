/*
 * investigate readelf - find the sections
 * starting point: man readelf
 * compile: gcc  04_random_program.c -o out_04_random_program
 * run: ./out_04_random_program.c
 *
 */
#include <stdio.h>
#include <string.h>

int C=100;


int cube(int number_in, int get_counter)
{
        static counter=0;
        counter=counter+1;
        if (get_counter)
                return counter;
        else
                return number_in*number_in*number_in;
}


int main()
{
        char A[8];
        unsigned short B = 1979;
        int output;

        strncpy(A, "short",6);
        output=cube(2,0);
        printf("Value output: %d\n",output);
        output=cube(3,0);
        printf("Value output: %d\n",output);
        output=cube(4,1);
        printf("Value output: %d\n",output);

        printf("Value B: %d\n",B);
        printf("Value C: %d\n",C);
}
