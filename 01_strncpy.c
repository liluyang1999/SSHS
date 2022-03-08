/*
 * Fix the code!
 * compile: gcc 01_strncpy.c -o out_01_strncpy
 * run: ./out_01_strncpy
 */

#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
    char tree[5];
    char elephant[10];

    strcpy(tree,"tree");
    strcpy(elephant,"elephant");

    // I've changed my mind - I want elephant to contain tree
    strncpy(elephant,tree,strlen(tree));

    printf ("Elephant contains: %s\n",elephant);

    return 0;
}

