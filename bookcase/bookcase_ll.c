#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "neillsimplescreen.h"

#define MAXW 9
#define MAXH 9
#define MAXSTR 100
/*Alter for a bigger list to handle bigger bookcases if needed*/
#define MAXLIST 10000000
/* Note: Placehold must be negative to work*/
#define PLACEHOLD -1

typedef enum bool {true, false} bool;

typedef struct list {
  char bc[MAXW][MAXH];
  struct list *next;
} list;

/************* Functions for bring in files *************/
int get_dim(char* file, char get);
void get_bc_fgets(char* file, int height, int width, char a[MAXH][MAXW]);
void gtbc_and_chk_file(char* file, int height, int width, char a[MAXH][MAXW]);

/************ Functions regarding solution ***************
int printnumofsol(list* a, int solnum);
void printsol(list* a, int height, int width, int sol);*/

/************* Functions for manipulating array of structure ************/
bool createlist(list* a, list* b, int height, int width);
bool childrenhpy(list* a, char b[MAXH][MAXW], int height, int width);
/*int childrenct(char b[MAXH][MAXW], int height, int width);*/
void copytostruct(list* a, char b[MAXH][MAXW], int height, int width);
void clearlist(list* a);

/************* Functions for bookcase manipulation ***********/
void jumbleline(char a[MAXH][MAXW], int height, int width, int row);
bool hasspace(char a[MAXH][MAXW], int width, int row);
void movebook(char a[MAXH][MAXW], int width, int currow, int tgtrow);
void clearrightval(char a[MAXH][MAXW], int width, int row);
void copybook(char a[MAXH][MAXW], int currow, int tgtrow, int width);
int spacecoord(char a[MAXH][MAXW], int width, int row);
int rightmostcoord(char a[MAXH][MAXW], int width, int row);
char rightmostval(char a[MAXH][MAXW], int width, int row);
void chgtoupper(char a[MAXH][MAXW], int height, int width);

/******** Functions for determining if bookcase is happy *********/
bool isbchappy(char a[MAXH][MAXW], int height, int width);
bool bkondifrow(char a[MAXH][MAXW], int height, int width);
bool samecolourrow(char a[MAXH][MAXW], int height, int width, int row);
bool unexpbook(char a[MAXH][MAXW], int height, int width);
bool checkfull(char a[MAXH][MAXW], int height, int width);

/********** Functions for copying/checking/printing **************/
bool chkdimtoosmall(char a[MAXH][MAXW], int height, int width);
bool chkdimtoobig(char a[MAXH][MAXW], int height, int width);
void cpyoneDtwoDarr(char a[MAXW][MAXH], char* z, int row, int width);
void copyarr(char a[MAXH][MAXW], char b[MAXH][MAXW], int width, int height);
void printarr(char a[MAXH][MAXW], int height, int width);
bool saferead(int column, int row, int height, int width);
void printcolour(char a[MAXH][MAXW], int colct, int rowct);

void test();

int main(int argc, char **argv){
  int h=0, w=0;
  char arr[MAXH][MAXW] = {0};
  list *start, *current;
  start = current = calloc(1, sizeof(struct list));
  if(start == NULL){
    printf("Couldn't allocate space");
    exit(EXIT_FAILURE);
  }
  /*test();*/

  if(argc < 4 && argc > 1){
    h=get_dim(argv[1], 'H');
    w=get_dim(argv[1], 'W');
    gtbc_and_chk_file(argv[1], h, w, arr);

    /*Copy array into first element of list*/
    copyarr(start -> bc, arr, w, h);

    /*Create the list and output the final number of the list*/
    if(createlist(start, current, h, w) == true){
      printf("Bookcase happy!");
    }
    /*Placeholder returned means no happy bookcase found*
    if(sol != PLACEHOLD){
      printf("%d\n\n", printnumofsol(a, sol));
    }
  }
  if(argc == 3){
    if(strcmp(argv[2], "verbose") == 0){
      if(sol != PLACEHOLD){
        printsol(a, h, w, sol);
      }
    }
    else{
      printf("Third argument was not 'verbose'");
    }*/
  }
  if(argc < 2){
    printf("ERROR: Incorrect usage, try e.g. %s filename (with or without) verbose\n"
    , argv[0]);
  }
  return 0;
}

/*Function to bring in bookcase then perform checks and convert to upper case
 if dimensions differ*/
void gtbc_and_chk_file(char* file, int height, int width, char a[MAXH][MAXW]){


  /*Convert to array height width before using in saferead*/
  int arrh, arrw;
  arrh= height-1;
  arrw= width-1;

  if(saferead(arrh, arrw, MAXH, MAXW) != true){
    printf("Bookcase is bigger than 9x9, please try a smaller bookcase\n");
    exit(EXIT_FAILURE);
  }
  get_bc_fgets(file, height, width, a);
  if(chkdimtoosmall(a, height, width) == true){
      printf("Dimensions stated are smaller than expressed bookcase\n");
    exit(EXIT_FAILURE);
  }
  if(chkdimtoobig(a, height, width) == true){

      printf("Dimensions stated are larger than expressed bookcase\n");
    exit(EXIT_FAILURE);
  }
  chgtoupper(a, height, width);
}

/* Function to get dimensions from file*/
int get_dim(char* file, char get){
  char z[MAXW] = {0};

  FILE *fp;

  int h, w, i, c;
  c=0, i=0;

  fp=fopen(file, "r");
  if(fp == NULL){
    printf("File will not open\n");
    exit(EXIT_FAILURE);
  }

  while(c != EOF && c != '\n'){
     c = (z[i++] = fgetc(fp));

     if(z[0] == '\n'){
       printf("Unexpected first line in file! Please check");
       exit(EXIT_FAILURE);
     }
     else{
       sscanf(z, "%d %d", &h, &w);
     }
  }

  fclose(fp);

  /*Specify if you want Height or Width returned*/
  if(get == 'H'){
    return h;
    }
  else if(get == 'W'){
    return w;
    }
  return 0;
}

/*Function to get bookcase from file*/
void get_bc_fgets(char* file, int height, int width, char a[MAXH][MAXW]){
  char z[MAXSTR] = {0};

  FILE *fp;

  int j, hoff, aryrow;
  hoff=height+1;

  fp=fopen(file, "r");
  if(fp == NULL){
    printf("File will not open\n");
    exit(EXIT_FAILURE);
  }

  for(j=0; j<hoff; j++){
    if(fgets(z, MAXSTR, fp) == NULL){
      j=hoff;
    }
    if(j>0 && j<hoff){

      /* Needs to be 1 less than the filerow to make up for first row
      containing co-ordinates*/
      aryrow=j-1;
      cpyoneDtwoDarr(a, z, aryrow, width);
    }
  }

  fclose(fp);
}

/*Function to copy 1D array into 2D array row*/
void cpyoneDtwoDarr(char a[MAXW][MAXH], char* z, int row, int width){
  int i;

  for(i=0; i < width; i++){
    a[row][i]= z[i];
  }
}

/*Print solutions
void printsol(list* a, int height, int width, int solnum){
  int i;

  i=solnum;
  while(a[i].p != PLACEHOLD){
    printarr(a[i].bc, height, width);
    i=a[i].p;
  }
  printarr(a[0].bc, height, width);
}*/

/*Get count of how many bookcases will be printed if solution found
int printnumofsol(list* a, int solnum){
  int i, count=1;
  i=solnum;

  while(a[i].p != PLACEHOLD){
    count++;
    i=a[i].p;
  }

  return count;
}*/

/*Function to create entire list up to MAXLIST and return the point at which
either a solution is found or the max number the list gets to. This Function
is untestable*/
bool createlist(list* a, list* b, int height, int width){
  int totc, k, end, c;
  bool chkhpy = false;

  totc=0;
  k=0;
  /*Create a maximum end so we don't go over the edge*/
  end= MAXLIST - (height*(height-1));

  /*Run checks on initial bookcase*/
  if(k==0){
    /*Check bookcase isn't initially happy*/
    if(isbchappy(a-> bc, height, width) == true){
      chkhpy=true;
      return chkhpy;
    }
    /*Check bookcase isn't full*/
    else if(checkfull(a-> bc, height, width) == true){
      printf("No solution?\n");
      chkhpy=false;
      return chkhpy;
    }
  }
  while(a -> next != NULL || chkhpy != true){

    /*Store as next*/
    b -> next = calloc(1, sizeof(list));
    if(b -> next == NULL){
      printf("Couldn't allocate space\n");
      exit(EXIT_FAILURE);
    }
    b = b -> next;

    /*Create children*/
    chkhpy=childrenhpy(b, b-> bc, height, width);
  }
  /* If last value isn't true then no bookcases found*/
  if(chkhpy == false){
    printf("No solution?\n");
  }
  return chkhpy;
}

/* Function to loop through every child bookcase, store it and
output if a happy bookcase is found*/
bool childrenhpy(list* a, char b[MAXH][MAXW], int height, int width){
  int j, k, swprow, count=0;
  char arr[MAXH][MAXW];

  for(j=0; j<height; j++){
    /*Create a target row to move a book to*/
    swprow=j+1;

    for(k=0; k < height-1; k++){
      /*If the target row is ever out of bounds then reset to first row*/
      if(swprow == height){
        swprow = 0;
      }
      /*Create a copy of the original bookcase so loop resets to only this bookcase*/
      copyarr(arr, b, width, height);

      /*Only create a bookcase if there is a book on current
       row to move or target row has space*/
      if(spacecoord(arr, width, j) != 0 && hasspace(arr, width, swprow) == true){
        count++;
        movebook(arr, width, j, swprow);

        /*Copy into structure number of child bookcases found*/
        copytostruct(a, arr, height, width);

        /*Store as next*/
        a -> next = calloc(1, sizeof(list));
        if(a -> next == NULL){
          printf("Couldn't allocate space");
          exit(EXIT_FAILURE);
        }
        a = a -> next;

        if(isbchappy(arr, height, width) == true){
          return true;
        }
      }
      swprow++;
    }
  }
  return false;
}

/*Function to count how many children produced
int childrenct(char b[MAXH][MAXW], int height, int width){
  int j, k, swprow, count=0;
  char arr[MAXH][MAXW];

  for(j=0; j<height; j++){
    /*Create a target row to move a book to
    swprow=j+1;

    for(k=0; k < height-1; k++){

      /*If the target row is ever out of bounds then reset to first row
      if(swprow == height){
        swprow = 0;
      }
      /*Create a copy of the original bookcase so loop resets to only this bookcase
      copyarr(arr, b, width, height);

      /*Only create a bookcase if there is a book on current
       row to move or target row has spac
      if(spacecoord(arr, width, j) != 0 && hasspace(arr, width, swprow) == true){
        count++;
        movebook(arr, width, j, swprow);

        if(isbchappy(arr, height, width) == true){
          return count;
        }
      }
      swprow++;
    }
  }
  return count;
}*/

/* Function to store array in structure*/
void copytostruct(list* a, char b[MAXH][MAXW], int height, int width){
  int i, j;

  for(j=0; j<height; j++){
    for(i=0; i<width; i++){
        a -> bc[j][i]=b[j][i];
    }
  }
}

/*Function to move a book to another row*/
void movebook(char a[MAXH][MAXW], int width, int currow, int tgtrow){

  if(hasspace(a, width, tgtrow) == true && spacecoord(a, width, currow) != 0){
    copybook(a, currow, tgtrow, width);
    clearrightval(a, width, currow);
  }
}

/* Function to blank out the rightmost value*/
void clearrightval(char a[MAXH][MAXW], int width, int row){
  int i, pt;

  /* Add placeholder*/
  pt=PLACEHOLD;

  for(i=0; i<width; i++){
    if(a[row][i] != '.'){
      pt = i;
    }
  }
  if(pt != PLACEHOLD){
    a[row][pt] = '.';
  }
}

/*Function to copy a book from a specified row to another specified row*/
void copybook(char a[MAXH][MAXW], int currow, int tgtrow, int width){
  char book;
  int space;

  /* Find colour of book from current row*/
  book=rightmostval(a, width, currow);

  /* Only copy if a book is found */
  if(book != '.'){
    /* Get x co-ordinate of leftmost space on the target row*/
    space=spacecoord(a, width, tgtrow);

    /*Set target row of leftmost space to book being moved*/
    a[tgtrow][space]=book;
  }
}

/*Function to return the left most x coordinate where the bookshelf has a space*/
int spacecoord(char a[MAXH][MAXW], int width, int row){
  int i, pt;

  /*Add placeholder*/
  pt=PLACEHOLD;

  /* Count backwards from width -1 for left most val */
  for(i=width-1; i>=0; i--){
    if(a[row][i] == '.'){
      pt = i;
    }
  }
  return pt;
}

/*Function to return the right most value of a row besides '.'*/
char rightmostval(char a[MAXH][MAXW], int width, int row){
  int i, pt;

  /* Add placeholder*/
  pt=PLACEHOLD;

  for(i=0; i<width; i++){
    if(a[row][i] != '.'){
      pt = i;
    }
  }
  if(pt == PLACEHOLD){
    return 0;
  }
  else{
    return a[row][pt];
  }
}

/* Function to state if a row has space for a book*/
bool hasspace(char a[MAXH][MAXW], int width, int row){

  if(a[row][width-1] == '.'){
    return true;
  }
  return false;
}

/* Function to tell if the bookcase is happy */
bool isbchappy(char a[MAXH][MAXW], int height, int width){
  int j, count=0;

  for(j=0; j < height ; j++){
       if(samecolourrow(a, height, width, j) == true){
         count++;
       }
  }
  /* Bookcase is happy if all rows have same colour and
   no same coloured book elsewhere*/
  if(count == height && bkondifrow(a, height, width) == false){
    return true;
  }
  else{
    return false;
  }
}

/*Function to return if a row has all same colour books, NOTE: Returns false
if row > height regardless*/
bool samecolourrow(char a[MAXH][MAXW], int height, int width, int row){
  /*Note: Count must start at 1 to account
  for saferead stopping once i+1 exceeds width*/
  int i, count=1;

  for(i=0; i<width; i++){
    if(saferead(i+1, row, height, width) == true){
      if(a[row][i] == a[row][i+1] || a[row][i+1] == '.'){
        count++;
      }
    }
  }
  if(count == width){
    return true;
  }
  else{
    return false;
  }
}

/*Function to check if there are any similar colour books on other rows*/
bool bkondifrow(char a[MAXH][MAXW], int height, int width){
  int i, j, k, m, othrow;

  /* Loop amount of rows*/
  for(j=0; j<height; j++){

    othrow=j+1;

    /*Loop amount of comparison rows*/
    for(k=0; k< height - 1; k++){

      /* If past height of array then loop back around */
      if(othrow == height){
        othrow = 0;
      }

      /*Loop for each book on current row*/
      for(m=0; m< width; m++){

        /*Loop for each book on other row*/
        for(i=0; i<width; i++){
          if(a[j][m] != '.' && a[j][m] == a[othrow][i]){
            return true;
          }
        }
      }

      othrow++;
    }
  }
  return false;
}

/*Function to flag if a book is not of the specified colours*/
bool unexpbook(char a[MAXH][MAXW], int height, int width){
  int i, j;

  for(j=0; j<height; j++){
    for(i=0; i<width; i++){
      if(a[j][i] != 'K' && a[j][i] != 'R' && a[j][i] != 'G' && a[j][i] != 'Y' &&
         a[j][i] != 'B' && a[j][i] != 'M' && a[j][i] != 'C' && a[j][i] != 'W' &&
         a[j][i] != '.'){
           return true;
         }
    }
  }
  return false;
}

/*Function to make whole bookcase upper case*/
void chgtoupper(char a[MAXH][MAXW], int height, int width){
  int i, j;

  for(j=0; j<height; j++){
    for(i=0; i<width; i++){
      if(islower((int)a[j][i]) != 0){
        a[j][i] = (char)toupper((int)a[j][i]);
      }
    }
  }

}

/*Function to check if bookcase is full*/
bool checkfull(char a[MAXH][MAXW], int height, int width){
  int j, truew, count;
  count=0;
  truew= width - 1;

  for(j=0; j<height; j++){
    if(a[j][truew] != '.'){
      count++;
    }
  }
  if(count == height){
    return true;
  }
  return false;
}

/*Function to check if the dimensions given are too small for bookcase*/
bool chkdimtoosmall(char a[MAXH][MAXW], int height, int width){
  int i, j;

  for(j=height; j<MAXH; j++){
    for(i=width; i<MAXW; i++){
      if(a[j][i] != '\0'){
        return true;
      }
    }
  }

  return false;
}

/*Function to check if the dimensions given are too small for bookcase*/
bool chkdimtoobig(char a[MAXH][MAXW], int height, int width){
  int i, j;

  for(j=0; j<height; j++){
    for(i=0; i<width; i++){
      if(a[j][i] == '\0'){
        return true;
      }
    }
  }
  return false;
}

/*Function to copy array into another*/
void copyarr(char a[MAXH][MAXW], char b[MAXH][MAXW], int width, int height){
  int i, j;

  for(j=0; j<height; j++){
    for(i=0; i<width; i++){

    a[j][i] = b[j][i];
    }
  }
}

/*Function to print in colour*/
void printcolour(char a[MAXH][MAXW], int colct, int rowct){
  switch(a[colct][rowct]){
    case 'K':
      neillfgcol(black);
      /*Change background colour so black is visible*/
      neillbgcol(white);
      printf("%c", a[colct][rowct]);

      /*Change background colour back to default*/
      neillbgcol(black);
      break;
    case 'R':
      neillfgcol(red);
      printf("%c", a[colct][rowct]);

      break;
    case 'G':
      neillfgcol(green);
      printf("%c", a[colct][rowct]);

      break;
    case 'Y':
      neillfgcol(yellow);
      printf("%c", a[colct][rowct]);

      break;
    case 'B':
      neillfgcol(blue);
      printf("%c", a[colct][rowct]);

      break;
    case 'M':
      neillfgcol(magenta);
      printf("%c", a[colct][rowct]);

      break;
    case 'C':
      neillfgcol(cyan);
      printf("%c", a[colct][rowct]);

      break;
    default:
      neillfgcol(white);
      printf("%c", a[colct][rowct]);

      break;
  }
}
/* Function to print 2D char array */
void printarr(char a[MAXH][MAXW], int height, int width){
  int i, j;

  for(j=0; j<height; j++){
    for(i=0; i<width; i++){


        printcolour(a, j, i);
    }
    printf("\n");
  }
  printf("\n");
}

/*Function to make sure you stay in bounds*/
bool saferead(int column, int row, int height, int width){
  if(column<0 || row<0 || column >= width || row >= height){
     return false;
  }
  return true;
}

void clearlist(list* a){
/*Function to clear array of structure 'list'*/
  memset(a, 0, sizeof(list));
}
