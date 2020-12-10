/*#include "../testassoc.c"*/
#include "specific.h"
#include "../assoc.h"

int _hashstr(assoc* a,  void* key);
int _hashstr_probe(assoc* a, void* key);
void _hashstrprint(assoc* a);
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

  if(keysize >= 0){
    a -> keysize = keysize;

    if(keysize == 0){
      a -> dataptr = ncalloc(INITSIZE, sizeof(char)*MAXSTR);
      a -> keyptr = ncalloc(1, sizeof(char)*MAXSTR);
    }
    else{
      a -> dataptr = ncalloc(INITSIZE, sizeof(int));
      a -> keyptr = ncalloc(1, sizeof(int));
    }
  }
  else{
    printf("Keysize must be positive!\n");
    exit(EXIT_FAILURE);
  }

  a -> nfilled = 0;
  a -> capacity = INITSIZE;

  return a;
}

/*
   Insert key/data pair
   - may cause resize, therefore 'a' might
   be changed due to a realloc() etc.*/

/*void assoc_insert(assoc** a, void* key, void* data){

}*/

/*
   Returns the number of key/data pairs
   currently stored in the table

unsigned int assoc_count(assoc* a){

}

/*
   Returns a pointer to the data, given a key
   NULL => not found

void* assoc_lookup(assoc* a, void* key){

}


/* Free up all allocated space from 'a' */
void assoc_free(assoc* a){
  if(a -> dataptr != NULL){
    free(a -> dataptr);
  }
  if(a -> keyptr != NULL){
    free(a -> keyptr);
  }
  if(a != NULL){
    free(a);
  }
}

/******************** Unofficial Functions ************************/
int _hashstr(assoc* a, void* key){
  char str[MAXSTR];
  int c, i=0;
  unsigned long hash=5381;

  /* Create copy of void pointer into a char pointer*/
  strcpy(str, (char*)key);
  printf("%s\n", str);

  /********** Hash function used from lecture notes *****************/
  while((c = (str[i++]))){
    hash= 33 * hash ^ c;
  }
  return (int)(hash%a->capacity);
}

int _hashstr_probe(assoc* a, void* key){

}

/*void _hashint(void* key){

}*/

unsigned int _nrstlowprime(unsigned int n){
  while(_isprime(n) == false){
    n--;
  }
  return n;
}

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

/*void _hashstrprint(assoc* a){
  int i;
  char* str = ncalloc(a -> capacity, sizeof(char)*MAXSTR);
  strcpy(str, (char*)a -> dataptr);

  while((*str[i++]))
    printf("%s\n", str);
  }
}*/

void _test(){
  assert(_isprime(2));
  assert(!_isprime(1));
  assert(!_isprime(4));
  assert(_isprime(7));
  assert(_isprime(400009));

  assert(_nrstlowprime(4)==3);
  assert(_nrstlowprime(400010)==400009);

}
