/*
 * compile: gcc -m32 05_where_are_they.c -o out_05_where_are_they
 * run: ./out_03_simple_string_overflow
 * using %p with printf - this prints the POINTER value
 * (ie which memory address that variable is located)
 * Have a play - can you see the difference in locations and know why
 */
#include <stdio.h>
#include <malloc.h>
#include <string.h>

// value 100 stored in data
int x = 100;

int main()
{
         // Stored in stack
         int a = 2;
         float b = 2.5;

         // Uninitialised static variable goes to BSS
         static int y;

         // allocate memory on heap
         int * ptr = (int*) malloc(2*sizeof(int)) ;

         // values 102 and 6 stored on heap
         ptr[0] = a+x;
         ptr [1] = 6;


        printf ("The pointer for x       points to memory location: %p\n",&x);
        printf ("The pointer for a       points to memory location: %p\n",&a);
        printf ("The pointer for b       points to memory location: %p\n",&b);
        printf ("The pointer for y       points to memory location: %p\n",&y);
        printf ("The pointer for ptr     points to memory location: %08p\n",ptr);
        printf ("The pointer for ptr[0]  points to memory location: %p\n",&ptr[0]);
        printf ("The pointer for ptr[1]  points to memory location: %p\n",&ptr[1]);

        char* hello = "Hello!";
        char* goodbye;
        goodbye=malloc(sizeof(char)*8);
        strcpy (goodbye, "Goodbye");

        printf ("The pointer for hello   points to memory location: %p\n",hello);
        printf ("The pointer for goodbye points to memory location: %p\n",goodbye);


        // deallocate memory on heap
        free(ptr);
        return 0;

}
