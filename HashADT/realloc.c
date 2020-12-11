/*#include "../testassoc.c"*/
#include "specific.h"
#include "../assoc.h"

int _hashnumstr(assoc* a,  void* key);
int _probenumstr(assoc* a, void* key);
int _hashstr(assoc* a, void* key, void* data);
void _hashprint(assoc* a, bool string);
bool _isprime(unsigned int n);
unsigned int _nrstlowprime(unsigned int n);
void _test();

/*
   Initialise the Associative array
   keysize : number of bytes (or 0 => string)
   This is important when comparing keys since
   we'll need to use either memcmp() or strcmp()
*/
assoc* assoc_init(int keysize){
  assoc* a;

  a = ncalloc(1, sizeof(assoc));
  a -> keysize = keysize;
  a -> nfilled = 0;
  a -> capacity = INITSIZE;
  a -> lookup = ncalloc(a -> capacity, sizeof(store));

  return a;
}

/*
   Insert key/data pair
   - may cause resize, therefore 'a' might
   be changed due to a realloc() etc.*/

void assoc_insert(assoc** a, void* key, void* data){
  /*Struct De-reference variable*/
  assoc* b = (*a);

  if(b -> keysize == 0){
    _hashstr(b, key, data);
  }
}

/*
   Returns the number of key/data pairs
   currently stored in the table
*/
unsigned int assoc_count(assoc* a){
  return a -> nfilled;
}

/*
   Returns a pointer to the data, given a key
   NULL => not found

void* assoc_lookup(assoc* a, void* key){

}


/* Free up all allocated space from 'a' */
void assoc_free(assoc* a){
  if(a -> lookup != NULL){
    free(a-> lookup);
  }
  if(a != NULL){
    free(a);
  }
}

/******************************************************************/
/******************** Unofficial Functions ************************/
/******************************************************************/

/*Function to return numeric hash value*/
int _hashnumstr(assoc* a, void* key){
  char str[MAXSTR];
  int c, i=0;
  unsigned long hash=5381;

  /* Create copy of void pointer into a char pointer*/
  strcpy(str, (char*)key);

  /*Hash function used from lecture notes */
  while((c = (str[i++]))){
    hash= 33 * hash ^ c;
  }
  hash=hash%a->capacity;
  return (int)(hash);
}

/*Function to create the probe hash number*/
int _probenumstr(assoc* a, void* key){
  char str[MAXSTR];
  int c, i=0;
  unsigned long hash=5381;

  /* Create copy of void pointer into a char pointer*/
  strcpy(str, (char*)key);

  /*Hash function used from lecture notes */
  while((c = (str[i++]))){
    hash= 33 * hash ^ c;
  }
  hash=(hash/ a-> capacity)%a->capacity;
  /*Precaution if probe is ever 0*/
  if(hash==0){
    hash++;
  }
  return (int)(hash);
}

/*Function perform hashing*/
int _hashstr(assoc* a, void* key, void* data){
  int n, p, filled;
  filled=a->nfilled;
  /* Get hashed key integer */
  n=_hashnumstr(a, key);
  printf("hash: %d ", n);

  /*Store key pointer and index pointer in structure if empty*/
  if(a -> lookup[n].keyptr == NULL){
    a -> lookup[n].keyptr = key;
    a -> lookup[n].dataptr = data;
    a -> nfilled++;
  }
  else{
    /* Create probe */
    p=_probenumstr(a, key);
    printf("probe: %d ", p);

    /*Add probe to hash number and get remainder*/
    while(filled == a -> nfilled){
      n=(n+p)%a -> capacity;
      printf("nexthash: %d ", n);
      if(a -> lookup[n].keyptr == NULL){
        a -> lookup[n].keyptr = key;
        a -> lookup[n].dataptr = data;
        a -> nfilled++;
      }
    }
  }
}

/*void _hashint(void* key){

}*/

/* Function to find the nearest prime lower than the number you send
 to the function*/
unsigned int _nrstlowprime(unsigned int n){
  while(_isprime(n) == false){
    n--;
  }
  return n;
}

/*Function to tell if the number you send to the function is prime*/
bool _isprime(unsigned int n){
  int i;

  if(n==1){
    return false;
  }
  else if(n==2){
    return true;
  }
  else{
    for(i=2; i<=(int)n/2; i++){
      if(n%i == 0){
        return false;
      }
    }
  }
  return true;
}

void _hashprint(assoc* a, bool string){
  int i;

  if(string==true){
    for(i=0; i< a->capacity; i++){
      printf("%s ", (char*)a -> lookup[i].keyptr);
    }
  }
  else{
    for(i=0; i< a->capacity; i++){
      printf("%d ", *((int*)a -> lookup[i].keyptr));
    }
  }
}

void _test(){
  assert(_isprime(2));
  assert(!_isprime(1));
  assert(!_isprime(4));
  assert(_isprime(7));
  assert(_isprime(400009));

  assert(_nrstlowprime(4)==3);
  assert(_nrstlowprime(400010)==400009);

}
