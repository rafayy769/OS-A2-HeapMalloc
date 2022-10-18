#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/time.h>
#include<assert.h>

#include "../include/my_allocator.h"

int ackermann(int a, int b);
/* used in "ackermann_main" */

void print_time_diff(struct timeval * tp1, struct timeval * tp2);
/* used in "ackermann" */

/*--------------------------------------------------------------------------*/
/* LOCAL VARIABLES */
/*--------------------------------------------------------------------------*/


unsigned long int num_allocations;


/*--------------------------------------------------------------------------*/
/* EXPORTED FUNCTIONS */
/*--------------------------------------------------------------------------*/

extern void ackermann_main() {
  /* This is function repeatedly asks the user for the two parameters
     "n" and "m" to pass to the ackermann function, and invokes the function.
     Before and after the invocation of the ackermann function, the 
     value of the wallclock is taken, and the elapsed time for the computation
     of the ackermann function is output.
  */

  int n, m; /* Parameter for the invocation of the ackermann function. */ 

  struct timeval tp_start; /* Used to compute elapsed time. */
  struct timeval tp_end;

  for (;;) {

    num_allocations = 0;

    printf("\n");
    printf("Please enter parameters n and m to ackermann function.\n");
    printf("Note that this function takes a long time to compute,\n");
    printf("even for small values. Keep n at or below 3, and mat or\n");
    printf("below 8. Otherwise, the function takes seemingly forever.\n");
    printf("Enter 0 for either n or m in order to exit.\n\n");

    printf("  n = "); scanf("%d", &n);
    if (!n) break;
     printf("  m = "); scanf("%d", &m);
    if (!m) break;

    printf("      n = %d, m = %d\n", n, m);

    assert(gettimeofday(&tp_start, 0) == 0);
    /* Assert aborts if there is a problem with gettimeofday.
       We rather die of a horrible death rather than returning
       invalid timing information! 
    */

    int result = ackermann(n, m);

    assert(gettimeofday(&tp_end, 0) == 0);
    /* (see above) */

    printf("Result of ackermann(%d, %d): %d\n", n, m, result); 

    printf("Time taken for computation : "); 
    print_time_diff(&tp_start, &tp_end);
    printf("\n");

    printf("Number of allocate/free cycles: %lu\n\n\n", num_allocations); 
  }
  
  printf("Reached end of ackermann program. Thank you for using it.\n");

}

void print_time_diff(struct timeval * tp1, struct timeval * tp2) {
  /* Prints to stdout the difference, in seconds and museconds, between two
     timevals. */

  long sec = tp2->tv_sec - tp1->tv_sec;
  long musec = tp2->tv_usec - tp1->tv_usec;
  if (musec < 0) {
    musec += 1000000;
    sec--;
  }
  printf(" [sec = %ld, musec = %ld] ", sec, musec);

}


int ackermann(int a, int b) {
/* This is the implementation of the ackermann function. The function itself is very
   function is very simple (just two recursive calls). We use it to exercise the
   memory allocator (see "my_alloc" and "my_free"). 
   For this, there are additional calls to "gettimeofday" to measure the elapsed
   time.
 */

  char * mem;

  /* The size "to_alloc" of the region to allocate is computed randomly: */
  int to_alloc =  ((2 << (rand() % 19)) * (rand() % 100)) / 100;
  if  (to_alloc < 4) to_alloc = 4;

  int result = 0, i;

  char c;

  mem = (char*)my_malloc(to_alloc * sizeof(char));

  num_allocations++;

  if (mem != NULL) {

    // generate a random byte to fulfill the allocated block of memory
    c = rand() % 128;
    memset(mem, c, to_alloc * sizeof(char));

    if (a == 0)
      result = b + 1;
    else if (b == 0)
      result = ackermann(a - 1, 1);
    else
      result = ackermann(a - 1, ackermann(a, b - 1) );

    // check memory value before deleting
    for (i = 0; i < to_alloc; i++) {
        if (mem[i] != c) {
            printf("Memory checking error!\n");
            break;
        }
    }

    my_free(mem);
  }

  return result;
}

