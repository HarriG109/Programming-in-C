#include "specific.h"
#include "assoc.h"

#define ARRSIZE 15
#define WORDS 370119
#define NUMRANGE 100000

char* strduprev(char* str);

int main(void)
{
  static char strs[10][50];
  static char list[10][50];
  assoc* a;
  int num, i;
  a = assoc_init(0);
  _test();

  strcpy(strs[0],"Test");
  strcpy(strs[1],"Testing");
  strcpy(strs[2],"Why");
  strcpy(strs[3],"Hi");
  strcpy(strs[4],"Trap");
  strcpy(strs[5],"Give");
  strcpy(strs[6],"Lunch");
  strcpy(strs[7],"Hunger");
  strcpy(strs[8],"Poop");
  strcpy(strs[9],"Yam");

  num=_hashstr(a, strs[0]);
  strcpy((char*)a -> keyptr, strs[0]);
  strcpy(list[num], strs[0]);
  printf("%s\n", (char*)a->keyptr);
  strcpy((char*)a -> dataptr, list[num]);

  for(i=0; i<a->capacity; i++){
    printf("%s\n", (char*)(a->dataptr+MAXSTR));
  }
  num=_hashstr(a, strs[1]);
  strcpy((char*)a -> keyptr, strs[1]);
  printf("%s\n", (char*)a->keyptr);
  num=_hashstr(a, strs[2]);
  num=_hashstr(a, strs[3]);

  /*_hashprint(a);*/


  assoc_free(a);
   /*static char strs[WORDS][50]={{0}};
   FILE *fp;
   char* tstr;
   void *p;
   unsigned int lngst;
   unsigned int j;
   assoc* a;
   static int i[WORDS];

   a = assoc_init(0);
   fp = nfopen("../../Data/Words/eng_370k_shuffle.txt", "rt");
   for(j=0; j<WORDS; j++){
      assert(assoc_count(a)==j);
      i[j] = j;
      if(fscanf(fp, "%s", strs[j])!=1){
         on_error("Failed to scan in a word?");
      }
      assoc_insert(&a, strs[j], &i[j]);
   }
   fclose(fp);

   /*
      What's the longest word that is still spelled
      correctly when reversed, but is not a palindrome ?


   lngst = 0;
   for(j=0; j<WORDS; j++){
      /* Longest
      if(strlen(strs[j]) > lngst){
         tstr = strduprev(strs[j]);
         /* Not a palindrome
         if(strcmp(tstr, strs[j])){
            /* Spelled correctly
            if((p = assoc_lookup(a, tstr))!=NULL){
               lngst = strlen(tstr);
               printf("%s <-> %s = %d (%d in the file)\n", tstr, strs[j], lngst, *(int*)p);
            }
         }
         free(tstr);
      }
   }
   assoc_free(a);

   /*
      Lets choose NUMRANGE numbers at random between 0 - (NUMRANGE-1)
      and hash them.  Then assoc_count() tells us how many are unique

   srand(time(NULL));
   a = assoc_init(sizeof(int));
   for(j=0; j<NUMRANGE; j++){
      i[j] = rand()%NUMRANGE;
      assoc_insert(&a, &i[j], NULL);
   }
   printf("%d unique numbers out of %d\n", assoc_count(a), j);*/

   return 0;
}

/* Make a copy, reversed */
char* strduprev(char* str)
{
   int i, j;
   char* t;
   j = strlen(str);
   t = ncalloc(j+1, 1); /* Add null char */
   strcpy(t, str);
   for(i=0, j--; i<j; i++,j--){
      /* Swap using bit-twiddling */
      t[i] ^= t[j];
      t[j] ^= t[i];
      t[i] ^= t[j];
   }
   return t;
}
