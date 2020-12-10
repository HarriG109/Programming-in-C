/*Specify multiplyer for resizing array*/
#define SCALEFACTOR 8

/*Specify initial size of hash table*/
#define INITSIZE 17
#define MAXSTR 100

typedef enum bool {false, true} bool;

/*Create structure containing a pointer to the data, a pointer to the key,
a number of elements stored, a maximum capacity*/

typedef struct assoc{
  void* dataptr;
  void* keyptr;
  int keysize;
  int nfilled;
  int capacity;
}assoc;

/*typedef struct data{
  void* data;
}data;*/
