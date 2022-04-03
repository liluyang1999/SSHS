#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

#define SIZE 64

/* structure to hold a string and a pointer to the next item

   For building a linked list:


   { "some string", next--}--->{ "next string"}, next--}--->{ "final string", 0 }
*/

struct item {
   char *s;
   struct item *next;
};

/* a shorthand for convenience: every time we write "item_t" it is like
   writing "struct item" */

typedef struct item item_t;

/* given a possibly null list, insert a new item onto the front of it which
   contains string s.  s must be non-null */

item_t *
insert_item (char *s, item_t *list) {
   item_t *new;

   assert (s);

   /* space for the new item */
   new = malloc (sizeof (item_t));
   if (!new) {
      perror ("malloc");
      exit (1);
   }

   /* copy the contents */
   new->s = strdup (s);
   new->next = list;
   /* return the new list */
   return new;
}

/* print a list out (it will be in the reverse order to how we
   built it). */
   
void
print_list (item_t *list, FILE *fd) {
   char message[SIZE], *messagep = &message[0];
   item_t *scan;
   int i = 0;
 
   for (scan = list; scan; scan = scan->next) {
      /* make a nicely formatted version */
      snprintf (messagep, SIZE, "item %03d: %s\n", ++i, scan->s);
      /* and print it out */
      fputs (message, fd);
   }
}

void
delete_list (item_t *list) {
   item_t *scan, *tmp;

   for (scan = list; scan; scan = tmp) {
      /* delete an item */
      tmp = scan->next;
      free (scan->s);
      free (scan);
   }
}


/* read lines from the input, and either add them to the list or
   (if it's null) print out the list so far */
   
item_t *
build_list (void) {
   char input[SIZE]; 
   item_t *list = 0;
   char *cr;
 
   while (fgets (input, sizeof(input), stdin)) {
     cr = strchr (input, '\n');
     if (cr) {
        *cr = '\0';
     }
     if (strlen (input) == 0) {
        print_list (list, stdout);
     } else {
        list = insert_item (input, list);
     }
   }
  return list;
}

int 
main (int argc, char **argv) {
   item_t *list;


   /* print a helpful message, but only if we are reading from a tty rather than a file */
   if (isatty (0)) {
      fputs ("type items one line at a time, blank lines will show the list so far\n", stderr);
      fputs ("type control-D in order to end your input\n", stderr);
   }
   list = build_list ();
   
   /* print it again as we exit */
   print_list (list, stdout);
   /* and clean up the memory we used */
   delete_list (list);
   exit (0);
}
      

