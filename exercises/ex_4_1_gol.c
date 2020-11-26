#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
/*#include "ex_4_1_gol.lif"*/

#define WIDTH 5
#define HEIGHT 5
#define MAXSTR 100

enum bool {false, true};
typedef enum bool bool;

void setland(int b[WIDTH][HEIGHT], char populate);
void printland(int b[WIDTH][HEIGHT]);
int numinhab(int b[WIDTH][HEIGHT], int loopcw, int loopch);
void setvalue(int b[WIDTH][HEIGHT], int loopcw, int loopch, int outval);
int isone(int b[WIDTH][HEIGHT], int loopcw, int loopch);
char* arrayloc(int i, int j, int maxwidth, int maxheight);
void alterinhab(int b[WIDTH][HEIGHT], int c[WIDTH][HEIGHT]);
void copyarray(int b[WIDTH][HEIGHT], int c[WIDTH][HEIGHT]);
void getfile_altercord(char* filename, int arr[WIDTH][HEIGHT]);

void test();

int main(int argc, char **argv){
  int x[WIDTH][HEIGHT];
  int y[WIDTH][HEIGHT];

  int k, generations;

  /* Create arrays of land */
  setland(x, '0');
  setland(y, '0');

  if( argc==3 ){
    getfile_altercord(argv[1], x);

    printland(x);

    generations= atoi(argv[2]);

    for(k=0; k < generations; k++){
        alterinhab(x, y);

        /*Copy array over original*/
        copyarray(x, y);
        printland(x);
    }
  }
  else{
    printf("ERROR: Incorrect usage, try e.g. %s 'Filename' 'Number of iterations'\n", argv[0]);
  }

  return 0;
}

void test(){
  /* Create array for testing */
  int f[WIDTH][HEIGHT];
  int centralw, centralh, maxw, maxh;

  /*Create an initial array for testing*/
  setland(f, '0');

  /* Create different places in the array to test */
  centralw= WIDTH/2;
  centralh= HEIGHT/2;
  maxw= WIDTH - 1;
  maxh= HEIGHT - 1;

  /* Test array is being produced and populating with value specified */
  assert( f[0][0] == '0');
  assert( f[centralw][centralh] == '0');
  assert( f[maxw][maxh] != ' ');

  /*Test of finding where we are in a 3x3 dimension array*/
  assert(strcmp(arrayloc(0, 0, 2, 2), "TL") == 0);
  assert(strcmp(arrayloc(1, 0, 2, 2), "T") == 0);
  assert(strcmp(arrayloc(2, 0, 2, 2), "TR") == 0);
  assert(strcmp(arrayloc(0, 1, 2, 2), "L") == 0);
  assert(strcmp(arrayloc(1, 1, 2, 2), "C") == 0);
  assert(strcmp(arrayloc(2, 1, 2, 2), "R") == 0);
  assert(strcmp(arrayloc(0, 2, 2, 2), "BL") == 0);
  assert(strcmp(arrayloc(1, 2, 2, 2), "B") == 0);
  assert(strcmp(arrayloc(2, 2, 2, 2), "BR") == 0);

  /* Test whether changing values of array works */
  setvalue(f, maxw, maxh, '@');
  setvalue(f, maxw-1, maxh, '*');
  setvalue(f, maxw, maxh-1, ' ');
  assert(f[maxw][maxh] == '@');
  assert(f[maxw-1][maxh] == '*');
  assert(f[maxw][maxh-1] == ' ');

  /* Reset array */
  f[maxw][maxh]='0';
  f[maxw-1][maxh]='0';
  f[maxw][maxh-1]='0';

  /* Set a few points in the array to 1 to test funtion */
  f[0][0]='1';
}

void getfile_altercord(char* filename, int arr[WIDTH][HEIGHT]){

  char z[MAXSTR];

  int finished = 0, n = 0;
  FILE *fp;
  int p, q;

  fp = fopen(filename, "r");
  if(fp == NULL){
    exit(EXIT_FAILURE);
  }
  while(finished == false){
     if(fgets(z, MAXSTR, fp) == NULL){
       fprintf(stderr, "Cannot open file\n");
       exit(EXIT_FAILURE);
     }
     else{
       finished = false;
       if(z[0] != '#'){
         if(sscanf(z, "%d %d", &p, &q) != 2){
           finished = true;
         }
       }
       n++;
       if(n != 1){
        arr[WIDTH/2 + p][HEIGHT/2 + q] = '1';
      }
     }
  }
  fclose(fp);
}

void alterinhab(int b[WIDTH][HEIGHT], int c[WIDTH][HEIGHT]){

  int i, j, n;

  for(j=0; j<HEIGHT; j++){
    for(i=0; i<WIDTH; i++){

      n=numinhab(b, i, j);
      switch(b[i][j]){
        case '0':

        if(n == 3){
          c[i][j]='1';
        }

        break;
        case '1':

        if(n == 2 || n == 3){
          c[i][j]='1';
        }
        else{
          c[i][j]='0';
        }
        break;
      }
    }
  }
}

void copyarray(int b[WIDTH][HEIGHT], int c[WIDTH][HEIGHT]){

  int k, l;

  for(l=0; l<HEIGHT; l++){
    for(k=0; k<WIDTH; k++){

      b[k][l]=c[k][l];
    }
  }
}

/* Function to see how many neighbors are inhabitants */
int numinhab(int b[WIDTH][HEIGHT], int loopcw, int loopch){
  int maxheight, maxwidth, count;

  count=0;
  maxheight=HEIGHT-1;
  maxwidth=WIDTH-1;

  /* Top left corner */
  if(strcmp(arrayloc(loopcw, loopch, maxwidth, maxheight), "TL") == 0){

  count=count+isone(b, loopcw, loopch + 1);
  count=count+isone(b, loopcw + 1, loopch);
  count=count+isone(b, loopcw + 1 , loopch + 1);
  }

  /* Top right corner */
  else if(strcmp(arrayloc(loopcw, loopch, maxwidth, maxheight), "TR") == 0){

  count=count+isone(b, loopcw, loopch + 1);
  count=count+isone(b, loopcw - 1, loopch);
  count=count+isone(b, loopcw - 1 , loopch + 1);
  }

  /* Bottom left corner */
  else if(strcmp(arrayloc(loopcw, loopch, maxwidth, maxheight), "BL") == 0){

  count=count+isone(b, loopcw, loopch - 1);
  count=count+isone(b, loopcw + 1, loopch - 1);
  count=count+isone(b, loopcw + 1 , loopch);
  }

  /* Bottom right corner */
  else if(strcmp(arrayloc(loopcw, loopch, maxwidth, maxheight), "BR") == 0){

  count=count+isone(b, loopcw, loopch - 1);
  count=count+isone(b, loopcw - 1, loopch);
  count=count+isone(b, loopcw - 1 , loopch - 1);
  }

  /* Top */
  else if(strcmp(arrayloc(loopcw, loopch, maxwidth, maxheight), "T") == 0){

  count=count+isone(b, loopcw - 1 , loopch);
  count=count+isone(b, loopcw + 1 , loopch);
  count=count+isone(b, loopcw - 1 , loopch + 1);
  count=count+isone(b, loopcw , loopch + 1);
  count=count+isone(b, loopcw + 1 , loopch + 1);
  }

  /* Left */
  else if(strcmp(arrayloc(loopcw, loopch, maxwidth, maxheight), "L") == 0){

  count=count+isone(b, loopcw, loopch - 1);
  count=count+isone(b, loopcw, loopch + 1);
  count=count+isone(b, loopcw + 1 , loopch - 1);
  count=count+isone(b, loopcw + 1, loopch);
  count=count+isone(b, loopcw + 1 , loopch + 1);
  }

  /* Right */
  else if(strcmp(arrayloc(loopcw, loopch, maxwidth, maxheight), "R") == 0){

  count=count+isone(b, loopcw - 1, loopch - 1);
  count=count+isone(b, loopcw - 1, loopch);
  count=count+isone(b, loopcw - 1 , loopch + 1);
  count=count+isone(b, loopcw, loopch - 1);
  count=count+isone(b, loopcw, loopch + 1);
  }

  /* Bottom */
  else if(strcmp(arrayloc(loopcw, loopch, maxwidth, maxheight), "B") == 0){

  count=count+isone(b, loopcw - 1, loopch - 1);
  count=count+isone(b, loopcw + 1, loopch - 1);
  count=count+isone(b, loopcw , loopch - 1);
  count=count+isone(b, loopcw - 1, loopch);
  count=count+isone(b, loopcw + 1 , loopch);
  }

  /* Central */
  else if(strcmp(arrayloc(loopcw, loopch, maxwidth, maxheight), "C") == 0){

  count=count+isone(b, loopcw, loopch - 1);
  count=count+isone(b, loopcw, loopch + 1);
  count=count+isone(b, loopcw + 1 , loopch - 1);
  count=count+isone(b, loopcw + 1, loopch);
  count=count+isone(b, loopcw + 1 , loopch + 1);
  count=count+isone(b, loopcw - 1 , loopch - 1);
  count=count+isone(b, loopcw - 1, loopch);
  count=count+isone(b, loopcw - 1 , loopch + 1);
  }
  return count;
}

/* Function to evaluate value of 2D array is 'z' and if so keeps a count */
int isone(int b[WIDTH][HEIGHT], int loopcw, int loopch){

  if(b[loopcw][loopch] == '1'){
    return 1;
  }
  return 0;
}

/* Function to re-map value of array */
void setvalue(int b[WIDTH][HEIGHT], int loopcw, int loopch, int outval){

  b[loopcw][loopch]= outval;
}


/*Function to evaluate where we are in the forest */
char* arrayloc(int i, int j, int maxwidth, int maxheight){

  /* Top left corner*/
  if(i == 0 && j == 0){
    return "TL";
  }
  /* Top right corner */
  else if(i == maxwidth && j == 0){
    return "TR";
  }
  /* Bottom left corner */
  else if(i == 0 && j == maxheight){
    return "BL";
  }
  /* Bottom right corner */
  else if(i == maxwidth && j == maxheight){
    return "BR";
  }
  /* Top */
  else if(maxwidth > i && i>0 && j == 0){
    return "T";
  }
  /* Left */
  else if(i == 0 && maxheight>j && j>0){
    return "L";
  }
  /* Right */
  else if(i == maxwidth && maxheight>j && j>0){
    return "R";
  }
  /* Bottom */
  else if(maxwidth>i && i>0 && j == maxheight){
    return "B";
  }
  /* Central */
  else if(maxwidth>i && i>0 && maxheight>j && j>0){
    return "C";
  }
  return 0;
}

/* Function to print the forest space */
void printland(int b[WIDTH][HEIGHT]){
  int i, j;

  for(j=0; j<HEIGHT; j++){
    for(i=0; i<WIDTH; i++){

      printf("%c", b[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

/* Function to create starting array */
void setland(int b[WIDTH][HEIGHT], char populate){
  int i, j;

  for(j=0; j<HEIGHT; j++){
    for(i=0; i<WIDTH; i++){

      b[i][j]=populate;
    }
  }
}
