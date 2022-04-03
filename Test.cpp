#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <unistd.h>
using namespace std;

int main() {
    char message[64];
    char* messagePtr = message;
    fgets(message, 10, stdin);
    printf("%s\n", message);
    return 0;
}


