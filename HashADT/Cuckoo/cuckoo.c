#include "specific.h"
#include "../assoc.h"

void* _lookup(assoc* a, void* key);
store* _bounce(assoc* a, void* key, void* data);
int _hashval1(assoc* a,  void* key);
int _hashval2(assoc* a,  void* key);
void _hash(assoc* a, void* key, void* data);
void _hashprntkey(assoc* a, char b);
void _resize(assoc** a);
int _comp(void* a, void* key, int bytesize);
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
    a -> lookup1 = ncalloc(a -> capacity, sizeof(store));
    a -> lookup2 = ncalloc(a -> capacity, sizeof(store));

    return a;
  }
  else{
    printf("Keysize is negative, please try again.\n");
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
    /*Times 2 as there are 2 hash tables now*/
    pctfld=(double)(*a) -> nfilled/ (*a) -> capacity*100*2;

    if(pctfld > PCNTFLD){
      _resize(a);
    }
    _hash((*a), key, data);
  }
  else{
    printf("Pointer to structure is null, try again\n");
    exit(EXIT_FAILURE);
  }
}

/*
   Returns the number of key/data pairs
   currently stored in the table
*/
unsigned int assoc_count(assoc* a){
  if(a != NULL){
    return a -> nfilled;
  }
  return 0;
}

/*
   Returns a pointer to the data, given a key
   NULL => not found
*/
void* assoc_lookup(assoc* a, void* key){

  if(key == NULL){
    printf("Trying to lookup a NULL pointer will result in NULL.\n");
    return NULL;
  }
  else if(a == NULL){
    printf("Trying to lookup a pointer in NULL structure will result in NULL.\n");
    return NULL;
  }
  else{
    return _lookup(a, key);
  }
  return NULL;
}


/* Free up all allocated space from 'a' */
void assoc_free(assoc* a){

  if(a != NULL){
    if(a -> lookup1 != NULL){
      free(a-> lookup1);
    }
    if(a -> lookup2 != NULL){
      free(a-> lookup2);
    }
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
  new -> capacity = b -> capacity*SCALEFACTOR;
  new -> lookup1 = ncalloc(new -> capacity, sizeof(store));
  new -> lookup2 = ncalloc(new -> capacity, sizeof(store));

  for(i=0; i< b -> capacity; i++){
    _hash(new, b -> lookup1[i].keyptr, b -> lookup1[i].dataptr);
    _hash(new, b -> lookup2[i].keyptr, b -> lookup2[i].dataptr);
  }
  *a = new;

  assoc_free(b);
}

/*Function to lookup*/
void* _lookup(assoc* a, void* key){
  int n, m;

  /* Get hashed key integers*/
  n=_hashval1(a, key);
  m=_hashval2(a, key);

  if(a -> lookup1[n].keyptr != NULL){
    /* Return if found*/
    if(_comp(a -> lookup1[n].keyptr, key, a -> keysize) == 0){
      return a -> lookup1[n].dataptr;
    }
  }
  if(a -> lookup2[m].keyptr != NULL){
    if(_comp(a -> lookup2[m].keyptr, key, a -> keysize) == 0){
      return a -> lookup2[m].dataptr;
    }
  }
  return NULL;
}

/*Function to perform hashing*/
void _hash(assoc* a, void* key, void* data){
  int i=0;
  store* copy;
  store* tmp;

  if(a != NULL){
    if(key != NULL){
      copy=_bounce(a, key, data);
      while(copy != NULL && i < 10){
        tmp=copy;
        copy=_bounce(a, copy -> keyptr, copy -> dataptr);
        free(tmp);
        i++;
      }
      free(copy);
    }
  }
}

/*Function to bounce to other hash table and back if needed*/
store* _bounce(assoc* a, void* key, void* data){
  int n, m;
  store* copy;
  store* copy2;
  copy = ncalloc(1, sizeof(store));
  copy2 = ncalloc(1, sizeof(store));

  /* Get hashed key integer */
  n=_hashval1(a, key);
  /*printf("hash 1: %d %s\n", n, (char*)key);*/

  /*Make copy of key in nth location in hash table1*/
  *copy = a -> lookup1[n];
  /*printf("Copy: %s\n", (char*)copy -> keyptr);*/

  /*Set new key in place of copied one*/
  a -> lookup1[n].keyptr = key;
  a -> lookup1[n].dataptr = data;

  /* If NULL then stop and increment*/
  if(copy -> keyptr == NULL){
    a -> nfilled++;
    free(copy);
    free(copy2);
    return NULL;
  }
  /*If same value then stop*/
  else if(_comp(copy -> keyptr, key, a -> keysize) == 0){
    free(copy);
    free(copy2);
    return NULL;
  }
  else{
    /*Hash copy for second table hash number*/
    m=_hashval2(a, copy -> keyptr);
    /*printf("hash 2: %d\n", m);*/

    *copy2 = a -> lookup2[m];
    /*printf("Copy 2: %s\n", (char*)copy2 -> keyptr);*/
    a -> lookup2[m].keyptr = copy -> keyptr;
    a -> lookup2[m].dataptr = copy -> dataptr;

    /* If NULL then stop*/
    if(copy2 -> keyptr == NULL){
      a -> nfilled++;
      free(copy);
      free(copy2);
      return NULL;
    }
    else if(_comp(copy2 -> keyptr,  copy -> keyptr, a -> keysize) == 0){
      free(copy);
      free(copy2);
      return NULL;
    }
    else{
      free(copy);
      return copy2;
    }
  }
}

/*Function to return numeric hash value for table 1*/
int _hashval1(assoc* a, void* key){
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

/*Function to return numeric hash value for table 2*/
int _hashval2(assoc* a, void* key){
  char str[MAXSTR];
  int c, i=0;
  unsigned long hash=3333;

  if(a -> keysize == 0){
    /* Create copy of void pointer into a char pointer*/
    strcpy(str, (char*)key);

    /*Hash function used from lecture notes */
    while((c = (str[i++]))){
      hash= 29 * hash ^ c;
    }
  }
  else{
    /* Create copy of void pointer into a char pointer*/
    memcpy(str, key, a -> keysize);

    /*HASH FUNCTION USED FROM LECTURE NOTES */
    while(i < a -> keysize){
      c = (str[i++]);
      hash= 29 * hash ^ c;
    }
  }

  hash=hash%a->capacity;
  return (int)(hash);
}

/* Compare int or string casted from void pointers against key, returns 0 if true */
int _comp(void* cmp, void* key, int bytesize){
  if(bytesize == 0){
    return strcmp((char*)(cmp), (char*)(key));
  }
  else if(bytesize > 0){
    return memcmp(cmp, key, bytesize);
  }
  else{
    printf("Keysize is negative, please check\n");
    return -1;
  }
}

/*Function to print key of different forms*/
void _hashprntkey(assoc* a, char b){
  int i, j;

  if(b == 'S'){
    for(i=0; i< a->capacity; i++){
      printf("%s ", (char*)a -> lookup1[i].keyptr);
    }
    printf("\n\n");
    for(j=0; j< a->capacity; j++){
      printf("%s ", (char*)a -> lookup2[j].keyptr);
    }
    printf("\n\n");
  }
  else if(b == 'I'){
    for(i=0; i< a->capacity; i++){
      /* Cannot print nulls for numerics */
      if(a -> lookup1[i].keyptr != NULL){
        printf("%d ", *(int*)a -> lookup1[i].keyptr);
      }
    }
    printf("\n\n");
    for(j=0; j< a->capacity; j++){
      /* Cannot print nulls for numerics */
      if(a -> lookup2[j].keyptr != NULL){
        printf("%d ", *(int*)a -> lookup2[j].keyptr);
      }
    }
    printf("\n\n");
  }
  else if(b == 'F'){
    for(i=0; i< a->capacity; i++){
      /* Cannot print nulls for numerics */
      if(a -> lookup1[i].keyptr != NULL){
        printf("%f ", *(float*)a -> lookup1[i].keyptr);
      }
    }
    printf("\n\n");
    for(j=0; j< a->capacity; j++){
      /* Cannot print nulls for numerics */
      if(a -> lookup2[j].keyptr != NULL){
        printf("%f ", *(float*)a -> lookup2[j].keyptr);
      }
    }
    printf("\n\n");
  }
  else if(b == 'D'){
    for(i=0; i< a->capacity; i++){
      /* Cannot print nulls for numerics */
      if(a -> lookup1[i].keyptr != NULL){
        printf("%f ", *(double*)a -> lookup1[i].keyptr);
      }
    }
    printf("\n\n");
    for(j=0; j< a->capacity; j++){
      /* Cannot print nulls for numerics */
      if(a -> lookup2[j].keyptr != NULL){
        printf("%f ", *(double*)a -> lookup2[j].keyptr);
      }
    }
    printf("\n\n");
  }
}

void _test(){
  int i;

  static char str1[2][10]={{"Cat"},
                           {"Cat"}};

  static char str2[23][10]={{"Lets"},
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
                           {"grade"},
                           {"please"},
                           {"I"},
                           {"need"},
                           {"this"},
                           {"cat"},
                           {"bat"},
                           {"fat"},
                           {"mat"},
                           {"rat"},
                           {"zat"}};

  static int nwords1[2]={1, 2};
  static int nwords2[23]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
                          18, 19, 20, 21, 22, 23};

  assoc* test0;
  assoc* test1;
  assoc* test2;

  assert((test0=assoc_init(-1)) == NULL);
  assert((test1=assoc_init(0)) != NULL);
  assert((test2=assoc_init(0)) != NULL);
  assert(test2 -> capacity == 17);
  assert(test2 -> nfilled == 0);
  assert(test2 -> keysize == 0);
  assert(test2 -> lookup1 != NULL);
  assert(test2 -> lookup2 != NULL);

  _hash(test1, str1[0], &nwords1[0]);
  _hash(test1, str1[1], &nwords1[1]);
  for(i=0; i<17; i++){
    _hash(test2, str2[i], &nwords2[i]);
  }

  _hashprntkey(test1, 'S');
  _hashprntkey(test2, 'S');

  assoc_free(test1);
  assoc_free(test2);


}
