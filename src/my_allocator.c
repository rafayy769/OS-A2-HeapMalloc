#include<stdlib.h>
#include "../include/my_allocator.h"

// Don't forget to implement "init_allocator()" and "release_allocator()"!


Addr my_malloc(unsigned int _length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */
  return malloc((size_t)_length);
}

int my_free(Addr _a) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */
  free(_a);
  return 0;
}

