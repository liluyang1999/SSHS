#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <unistd.h>
using namespace std;

int main() {
    char* p1 = (char*)malloc(50);
    fgets(p1, BUFSIZ, stdin);
    char* ch = strchr(p1, '\n');
    *ch = '\0';
    printf("%s\n", p1);

    return 0;
}


