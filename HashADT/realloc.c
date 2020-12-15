/*#include "../testassoc.c"*/
#include "specific.h"
#include "../assoc.h"

void* _lookupstr(assoc* a, void* key);
int _hashval(assoc* a,  void* key);
int _probeval(assoc* a, void* key);
void _hash(assoc* a, void* key, void* data);
void _hashprint(assoc* a, bool string);
void _resize(assoc** a);
int _comp(void* a, void* key, int bytesize);
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

  if(keysize >= 0){
    a = ncalloc(1, sizeof(assoc));
    a -> keysize = keysize;
    a -> nfilled = 0;
    a -> capacity = INITSIZE;
    a -> lookup = ncalloc(a -> capacity, sizeof(store));

    return a;
  }
  else{
    printf("Keysize is negative, please try again.");
    return NULL;
  }
}

/*
   Insert key/data pair
   - may cause resize, therefore 'a' might
   be changed due to a realloc() etc.
*/
void assoc_insert(assoc** a, void* key, void* data){

  if(a != NULL && *a != NULL){
    /*Create percentage that hash table is filled*/
    double pctfld;
    pctfld=(double)(*a) -> nfilled/ (*a) -> capacity*100;

    if(pctfld > 70){
      _resize(a);
    }
    _hash((*a), key, data);
  }
  else{
    printf("Pointer to structure is null, try again");
    exit(EXIT_FAILURE);
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
*/
void* assoc_lookup(assoc* a, void* key){

  if(key != NULL){
    return _lookupstr(a, key);
  }
  else{
    printf("Trying to lookup a NULL pointer will result in NULL.");
    return NULL;
  }
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

/* Resize hash table and rehash old data*/
void _resize(assoc** a){
  int i;

  /*Temporary pointer to free*/
  assoc* new;
  /*Struct De-reference variable*/
  assoc* b = (*a);

  new = ncalloc(1, sizeof(assoc));
  new -> keysize = b -> keysize;
  /*Reset 'nfilled' because nothing has been hashed yet*/
  new -> nfilled = 0;
  /* Make sure new capacity is prime */
  new -> capacity = _nrstlowprime(b -> capacity*SCALEFACTOR);
  new -> lookup = ncalloc(new -> capacity, sizeof(store));

  for(i=0; i< b-> capacity; i++){
    _hash(new, b -> lookup[i].keyptr, b -> lookup[i].dataptr);
  }
  *a = new;

  assoc_free(b);
}

/*Function to lookup*/
void* _lookupstr(assoc* a, void* key){
  int n, p;

  /* Get hashed key integer */
  n=_hashval(a, key);

  if(a -> lookup[n].keyptr != NULL &&
     _comp(a -> lookup[n].keyptr, key, a -> keysize) == 0){
    return a -> lookup[n].dataptr;
  }
  else{
    /* Create probe */
    p=_probeval(a, key);

    /* Lookup x times before we assume word is not found*/
    while(a -> lookup[n].keyptr != NULL){
      /*Add probe to hash number and get remainder*/
      n=(n+p)%a -> capacity;
      if(a -> lookup[n].keyptr != NULL &&
         _comp(a -> lookup[n].keyptr, key, a -> keysize) == 0){
        return a -> lookup[n].dataptr;
      }
    }
    return NULL;
  }
}

/*Function to perform hashing*/
void _hash(assoc* a, void* key, void* data){
  if(a != NULL){
    int n, p;
    bool stored = false;

    if(key != NULL){
      /* Get hashed key integer */
      n=_hashval(a, key);
      /*printf("hash: %d ", n);*/

      /*Store key pointer and index pointer in structure if empty*/
      if(a -> lookup[n].keyptr == NULL){
        a -> lookup[n].keyptr = key;
        a -> lookup[n].dataptr = data;
        a -> nfilled++;
      }
      else{
        /* If same number or string then overwrite datapointer */
        if(a -> lookup[n].keyptr != NULL &&
           _comp(a -> lookup[n].keyptr, key, a -> keysize) == 0){
          a -> lookup[n].dataptr = data;
          stored = true;
        }
        /* Create probe */
        p=_probeval(a, key);
        /*printf("probe: %d ", p);*/

        /* If an element has been stored in table then stop*/
        while(stored == false){
          /*Add probe to hash number and get remainder*/
          n=(n+p)%a -> capacity;
          /*printf("nexthash: %d ", n);*/
          /* If same number or string then overwrite datapointer only */
          if(a -> lookup[n].keyptr != NULL &&
             _comp(a -> lookup[n].keyptr, key, a -> keysize) == 0){
            a -> lookup[n].dataptr = data;
            stored = true;
          }
          else if(a -> lookup[n].keyptr == NULL){
            a -> lookup[n].keyptr = key;
            a -> lookup[n].dataptr = data;
            a -> nfilled++;
            stored = true;
          }
        }
      }
    }
  }
  else{
    printf("Structure is NULL, please check and try again!");
  }
}

/*Function to return numeric hash value*/
int _hashval(assoc* a, void* key){
  char str[MAXSTR];
  int c, i=0;
  unsigned long hash=5381;

  if(a -> keysize == 0){
    /* Create copy of void pointer into a char pointer*/
    strcpy(str, (char*)key);

    /*Hash function used from lecture notes */
    while((c = (str[i++]))){
      hash= 33 * hash ^ c;
    }
  }
  else{
    /* Create copy of void pointer into a char pointer*/
    memcpy(str, key, a -> keysize);

    /*HASH FUNCTION USED FROM LECTURE NOTES */
    while(i < a -> keysize){
      c = (str[i++]);
      hash= 33 * hash ^ c;
    }
  }

  hash=hash%a->capacity;
  return (int)(hash);
}

/*Function to create the probe number*/
int _probeval(assoc* a, void* key){
  char str[MAXSTR];
  int c, i=0;
  unsigned long hash=5381;

  if(a -> keysize == 0){
    /* Create copy of void pointer into a char pointer*/
    strcpy(str, (char*)key);

    /*PROBE FUNCTION USED FROM LECTURE NOTES */
    while((c = (str[i++]))){
      hash= 33 * hash ^ c;
    }
  }
  else{
    /* Create copy of void pointer into a char pointer*/
    memcpy(str, key, a -> keysize);

    /*PROBE FUNCTION USED FROM LECTURE NOTES */
    while(i < a -> keysize){
      c = (str[i++]);
      hash= 33 * hash ^ c;
    }
  }

  hash=(hash/ a-> capacity)%a->capacity;

  /*Precaution if probe is ever 0*/
  if(hash==0){
    hash++;
  }
  return (int)(hash);
}

/* Compare int or string function within assoc against key, returns 0 if true */
int _comp(void* cmp, void* key, int bytesize){
  if(bytesize == 0){
    return strcmp((char*)(cmp), (char*)(key));
  }
  else if(bytesize > 0){
    return memcmp(cmp, key, bytesize);
  }
  else{
    printf("Keysize is negative, please check");
    return -1;
  }
}

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
  static char str1[2][10]={{"Cat"},
                           {"Cat"}};

  static char str2[13][10]={{"Lets"},
                           {"test"},
                           {"all"},
                           {"my"},
                           {"functions"},
                           {"so"},
                           {"Neill"},
                           {"can"},
                           {"give"},
                           {"me"},
                           {"a"},
                           {"good"},
                           {"grade"}};

  static int nwords1[2]={1, 2};
  static int nwords2[13]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

  assoc* test1;
  assoc* test2;
  assoc* test3;

  assert((test1=assoc_init(-1)) == NULL);
  assert((test2=assoc_init(0)) != NULL);
  assert((test3=assoc_init(8)) != NULL);
  assert(test2 -> capacity == 17);
  assert(test2 -> nfilled == 0);
  assert(test2 -> keysize == 0);

  assert(_hashval(test2, str1[0]) >= 0);
  assert(_hashval(test2, str1[0]) <= 17);

  _hash(test2, str1[0], &nwords1[0]);
  assert(test2 -> nfilled == 1);
  _hash(test2, str1[1], &nwords1[1]);
  assert(test2 -> nfilled == 1);
  _hash(test3, str2[0], &nwords2[0]);
  assert(test3 -> nfilled == 1);
  _hash(test3, str2[1], &nwords2[1]);
  assert(test3 -> nfilled == 2);
  _hash(test3, str2[10], &nwords2[10]);
  assert(test3 -> nfilled == 3);
  /*_hashprint(test2, true);*/

  /* Don't need to free a it should return NULL*/
  assoc_free(test2);
  assoc_free(test3);

  assert(_isprime(2));
  assert(!_isprime(1));
  assert(!_isprime(4));
  assert(_isprime(7));
  assert(_isprime(400009));

  assert(_nrstlowprime(4)==3);
  assert(_nrstlowprime(400010)==400009);

}
