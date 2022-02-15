#include <stdio.h>
#include <string.h>


void foo()
{
	char c[128];
	printf ("Printing address of c: %p\n Press return to continue\n", c);

	gets(c);
}

int main(int argc, char **argv)
{
	foo();
	return 0;
}