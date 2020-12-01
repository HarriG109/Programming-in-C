#include <string.h>

#define CAPACITY 2
#define BITSPERBYTE 8

typedef enum bool {false, true} bool;

typedef struct boolarr{
  unsigned char *array;
  int maxsize;
  int numbits;
} boolarr;
