#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 10
#define SIZE 5
char* copy_input(char* user_supplied_string);

int main(int argc, char **argv)
{
    char tree[5];
    char elephant[10];

    strcpy(tree,"tree");
    strcpy(elephant,"elephant");

    // I've changed my mind - I want elephant to contain tree
    strncpy(elephant,tree,strlen(tree));

    for ( int i = 0; i < 10; i++) {
        printf("%c", elephant[i]);
    }
    printf("\n");

    printf ("Elephant contains: %s\n",elephant);

    return 0;
}

char* copy_input(char* user_supplied_string) {
    int i, dst_index;
    char* dst_buf = (char*) malloc(4 * sizeof(char));
    if ( MAX_SIZE <= strlen(user_supplied_string) ){
        printf("user string too long, die evil hacker!");
    }
    dst_index = 0;
    for ( i = 0; i < strlen(user_supplied_string); i++ ){
        if( '&' == user_supplied_string[i] ){
            dst_buf[dst_index++] = '&';
            dst_buf[dst_index++] = 'a';
            dst_buf[dst_index++] = 'm';
            dst_buf[dst_index++] = 'p';
            dst_buf[dst_index++] = ';';
        }
        else if ('<' == user_supplied_string[i] ){
        }
        else dst_buf[dst_index++] = user_supplied_string[i];
    }
    return dst_buf;
}

#define ACCEPT 1
#define REJECT 0
int func(int a, int b) {
    for (int c = 1; c <= a - 1; c++) {
        int temp = b * c;
        if (temp == a) {
            return 1;
        }
    }
    return 0;
}

