/*
 * Fix the code!
 * compile: gcc 01_strncpy.c -o out_01_strncpy
 * run: ./out_01_strncpy
 */

#include <string.h>
#include <stdio.h>
#include <windows.system.h>

int main(int argc, char **argv) {
    char tree[5];
    char elephant[10];

    strcpy(tree,"tree");
    strcpy(elephant,"elephant");

    // I've changed my mind - I want elephant to contain tree
    strncpy(elephant,tree,strlen(tree));

    printf ("Elephant contains: %s\n",elephant);

    char *string1 = "java -version";
    int *a;
    int a1 = 3;
    a = &a1;
    int *b;
    int b1 = 4;
    b = &b1;
    printf("%.16x, %.16x, %.16x, %.16x");

    return 0;
}

