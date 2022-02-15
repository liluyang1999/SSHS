#include <stdio.h>
#include <string.h>

void administrator()
{
	printf ("Congratulations - you are now an administrator!\n");
}

void foo(char *bar)
{
	char c[12];
	strcpy(c, bar); // no bounds checking
}

int main(int argc, char **argv)
{
	printf("Address of function administrator: %p\n",administrator);
	foo(argv[1]);
	printf ("You entered: %s\n",argv[1]);
	return 0;
}