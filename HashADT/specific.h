/*Specify multiplyer for resizing array*/
#define SCALEFACTOR 8

/*Specify initial size of hash table*/
#define INITSIZE 17
#define MAXSTR 100

typedef enum bool {false, true} bool;

/*Create structure containing a pointer to the data and a pointer to the key*/
typedef struct store{
  void* dataptr;
  void* keyptr;
}store;

/*Create structure containing a array of structs of the above, the keysize,
number of elements in the array of structs and max capacity*/
typedef struct assoc{
  store* lookup;
  int keysize;
  int nfilled;
  int capacity;
}assoc;
