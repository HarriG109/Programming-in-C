/*Specify multiplyer for resizing array*/
#define SCALEFACTOR 8

/*Specify initial size of hash table*/
#define INITSIZE 17

/*Specify max size of strings possible*/
#define MAXSTR 100

/*Specify percentage the hash table should be filled before resizing
(Must be between 20-80 to work, 50-70 region is optimal)*/
#define PCNTFLD 70

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
