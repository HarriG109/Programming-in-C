#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "neillsimplescreen.h"

#define HEIGHT 30
#define WIDTH 80
#define TREEPROB 200
#define FIREPROB (10*TREEPROB)
#define GENERATIONS 1000

enum bool {false, true};
typedef enum bool bool;

void setarray(int b[WIDTH][HEIGHT], char populate);
void printforest(int b[WIDTH][HEIGHT]);
bool treegr_lightn(int prob);
bool whatvalue(int b[WIDTH][HEIGHT], int loopcw, int loopch, int val);
void setvalue(int b[WIDTH][HEIGHT], int loopcw, int loopch, int outval);
void changevalue(int b[WIDTH][HEIGHT], int c[WIDTH][HEIGHT],
                    int loopcw, int loopch, int inval, int outval);
void spreadfire(int b[WIDTH][HEIGHT], int hassprd[WIDTH][HEIGHT],
                int loopcw, int loopch);
void treecatchfire(char c);
char* whereinforest(int i, int j, int maxwidth, int maxheight);
void simulateforest(int b[WIDTH][HEIGHT], int tree, int light);

void test();

int main(void){

  int a[WIDTH][HEIGHT];
  char start_val;
  int i;

  start_val=' ';

  /*Run tests*/
  test();

  /* Create land */
  setarray(a, start_val);

  /* Ue Neill's functions to wipe screen and default cursor to top */
  neillclrscrn();
  neillcursorhome();

  /* Create simulation i number of generations */
  for(i=0; i<GENERATIONS; i++){
    printf("Generation %d:\n", i);
    simulateforest(a, TREEPROB, FIREPROB);

    printforest(a);
    neillbusywait(0.2);
  }

  return 0;
}

void test(){
  /* Create array for testing */
  int f[WIDTH][HEIGHT];
  int null[WIDTH][HEIGHT];
  int centralw, centralh, maxw, maxh;

  /*Create an initial array for testing*/
  setarray(f, ' ');

  /* Create null array to avoid duplicate fires */
  setarray(null, '0');

  /* Create different places in the array to test */
  centralw= WIDTH/2;
  centralh= HEIGHT/2;
  maxw= WIDTH - 1;
  maxh= HEIGHT - 1;

  /* Test array is being produced and populating with value specified */
  assert( f[0][0] == ' ');
  assert( f[centralw][centralh] == ' ');
  assert( f[maxw][maxh] == ' ');

  /* Test of treegrowth and lightning strike */
  assert( treegr_lightn(1) == true);
  assert( treegr_lightn(100000) == false);

  /*Test of finding where we are in a 3x3 dimension array*/
  assert(strcmp(whereinforest(0, 0, 2, 2), "TL") == 0);
  assert(strcmp(whereinforest(1, 0, 2, 2), "T") == 0);
  assert(strcmp(whereinforest(2, 0, 2, 2), "TR") == 0);
  assert(strcmp(whereinforest(0, 1, 2, 2), "L") == 0);
  assert(strcmp(whereinforest(1, 1, 2, 2), "C") == 0);
  assert(strcmp(whereinforest(2, 1, 2, 2), "R") == 0);
  assert(strcmp(whereinforest(0, 2, 2, 2), "BL") == 0);
  assert(strcmp(whereinforest(1, 2, 2, 2), "B") == 0);
  assert(strcmp(whereinforest(2, 2, 2, 2), "BR") == 0);

  /*Populate initial array 3x3 trees with fire central for testing*/
  f[0][0]='@';
  f[0][1]='@';
  f[0][2]='@';
  f[1][0]='@';
  f[1][1]='*';
  f[1][2]='@';
  f[2][0]='@';
  f[2][1]='@';
  f[2][2]='@';

  /* Test whether values returned are true */
  assert(whatvalue(f, 0, 0, '@') == true);
  assert(whatvalue(f, 0, 0, '*') == false);
  assert(whatvalue(f, maxw, maxh, ' ') == true);
  assert(whatvalue(f, maxw, maxh, '*') == false);
  assert(whatvalue(f, 1, 1, '*') == true);
  assert(whatvalue(f, 1, 1, '@') == false);

  /* Test whether changing values of array works */
  setvalue(f, maxw, maxh, '@');
  setvalue(f, maxw-1, maxh, '*');
  setvalue(f, maxw, maxh-1, ' ');
  assert(f[maxw][maxh] == '@');
  assert(f[maxw-1][maxh] == '*');
  assert(f[maxw][maxh-1] == ' ');

  /* Test the changing of a cell from one thing to another */
  changevalue(f, null, 0, 0, '@', '*');
  assert(f[0][0] == '*');
  assert(null[0][0] == '1');
  changevalue(f, null, 2, 2, '@', '*');
  assert(f[2][2] == '*');
  assert(null[2][2] == '1');
  changevalue(f, null, 1, 1, '*', '@');
  assert(f[1][1] == '@');
  assert(null[1][1] == '1');
  changevalue(f, null, 1, 1, '@', '*');
  assert(f[1][1] == '*');
  assert(null[1][1] == '1');

  /* Reset values and test spreading of fire */
  f[1][1]='*';
  f[0][0]='@';
  f[2][2]='@';

  /* Reset null array */
  setarray(null, '0');

  spreadfire(f, null, 1, 1);

  assert(f[0][0] == '*');
  assert(f[0][1] == '*');
  assert(f[0][2] == '*');
  assert(f[1][0] == '*');
  assert(f[1][1] == ' ');
  assert(f[1][2] == '*');
  assert(f[2][0] == '*');
  assert(f[2][1] == '*');
  assert(f[2][2] == '*');

  /*Reset and populate initial array 2x2 trees
  with fire top corner for testing*/
  setarray(f, ' ');
  setarray(null, '0');

  f[0][0]='*';
  f[0][1]='@';
  f[1][0]='@';
  f[1][1]='@';

  spreadfire(f, null, 0, 0);

  assert(f[0][0] == ' ');
  assert(f[0][1] == '*');
  assert(f[1][0] == '*');
  assert(f[1][1] == '*');

  /*Reset and populate initial array 3x2 trees with
  fire centred but top of array for testing*/
  setarray(f, ' ');
  setarray(null, '0');

  f[0][0]='@';
  f[1][0]='*';
  f[2][0]='@';
  f[0][1]='@';
  f[1][1]='@';
  f[2][1]='@';

  spreadfire(f, null, 1, 0);

  assert(f[0][0] == '*');
  assert(f[1][0] == ' ');
  assert(f[2][0] == '*');
  assert(f[0][1] == '*');
  assert(f[1][1] == '*');
  assert(f[2][1] == '*');

}

/* Function to create starting array */
void setarray(int b[WIDTH][HEIGHT], char populate){
  int i, j;

  for(j=0; j<HEIGHT; j++){
    for(i=0; i<WIDTH; i++){

      b[i][j]=populate;
    }
  }
}

/* Function to print the forest space */
void printforest(int b[WIDTH][HEIGHT]){
  int i, j;

  for(j=0; j<HEIGHT; j++){
    for(i=0; i<WIDTH; i++){

      if(b[i][j] == ' '){
        neillfgcol(black);
        printf("%c", b[i][j]);
      }
      else if(b[i][j] == '@'){
        neillfgcol(green);
        printf("%c", b[i][j]);
      }
      else if(b[i][j] == '*'){
        neillfgcol(red);
        printf("%c", b[i][j]);
      }
      neillfgcol(white);
    }
    printf("\n");
  }
  printf("\n");
}

/* Function to simulate the forest */
void simulateforest(int b[WIDTH][HEIGHT], int tree, int light){
  int i, j;

  /* Create an array to check whether fire had already spread*/
  int x[WIDTH][HEIGHT];
  setarray(x, '0');

  for(j=0; j<HEIGHT; j++){
    for(i=0; i<WIDTH; i++){

      switch(b[i][j]){
        case '*':
          spreadfire(b, x, i, j);

          break;

        case ' ':
          if(treegr_lightn(tree) == true){
            b[i][j]='@';
          }
          break;

        case '@':

          if(treegr_lightn(light) == true){
            b[i][j]='*';
          }
          break;
      }
    }
  }
}


/* Function to spread fire, note that a '0' filled array of the same size
needs to be used which tracks whether the fire has already spread.
I apologise for the size of this function, I can't work out how to further
reduce it! */
void spreadfire(int b[WIDTH][HEIGHT], int hassprd[WIDTH][HEIGHT],
                int loopcw, int loopch){
  int maxheight, maxwidth;

  maxheight=HEIGHT-1;
  maxwidth=WIDTH-1;

  /* If fire hasn't yet spread then continue spreading */
  if(hassprd[loopcw][loopch] == '0'){

    /* Top left corner */
    if(strcmp(whereinforest(loopcw, loopch, maxwidth, maxheight), "TL") == 0){

      changevalue(b, hassprd, loopcw, loopch + 1, '@', '*');
      changevalue(b, hassprd, loopcw + 1, loopch, '@', '*');
      changevalue(b, hassprd, loopcw + 1, loopch + 1, '@', '*');
    }

    /* Top right corner */
    else if(strcmp(whereinforest(loopcw, loopch, maxwidth, maxheight), "TR") == 0){

      changevalue(b, hassprd, loopcw, loopch + 1, '@', '*');
      changevalue(b, hassprd, loopcw - 1, loopch, '@', '*');
      changevalue(b, hassprd, loopcw - 1, loopch + 1, '@', '*');
    }

    /* Bottom left corner */
    else if(strcmp(whereinforest(loopcw, loopch, maxwidth, maxheight), "BL") == 0){

      changevalue(b, hassprd, loopcw, loopch - 1, '@', '*');
      changevalue(b, hassprd, loopcw + 1, loopch -1, '@', '*');
      changevalue(b, hassprd, loopcw + 1, loopch, '@', '*');
    }

    /* Bottom right corner */
    else if(strcmp(whereinforest(loopcw, loopch, maxwidth, maxheight), "BR") == 0){

      changevalue(b, hassprd, loopcw, loopch - 1, '@', '*');
      changevalue(b, hassprd, loopcw - 1, loopch, '@', '*');
      changevalue(b, hassprd, loopcw - 1, loopch - 1, '@', '*');
    }

    /* Top */
    else if(strcmp(whereinforest(loopcw, loopch, maxwidth, maxheight), "T") == 0){

      changevalue(b, hassprd, loopcw - 1, loopch, '@', '*');
      changevalue(b, hassprd, loopcw + 1, loopch, '@', '*');
      changevalue(b, hassprd, loopcw - 1, loopch + 1, '@', '*');
      changevalue(b, hassprd, loopcw, loopch + 1, '@', '*');
      changevalue(b, hassprd, loopcw + 1, loopch + 1, '@', '*');
    }

    /* Left */
    else if(strcmp(whereinforest(loopcw, loopch, maxwidth, maxheight), "L") == 0){

      changevalue(b, hassprd, loopcw, loopch - 1, '@', '*');
      changevalue(b, hassprd, loopcw, loopch + 1, '@', '*');
      changevalue(b, hassprd, loopcw + 1, loopch - 1, '@', '*');
      changevalue(b, hassprd, loopcw + 1, loopch, '@', '*');
      changevalue(b, hassprd, loopcw + 1, loopch + 1, '@', '*');
    }

    /* Right */
    else if(strcmp(whereinforest(loopcw, loopch, maxwidth, maxheight), "R") == 0){

      changevalue(b, hassprd, loopcw - 1, loopch - 1, '@', '*');
      changevalue(b, hassprd, loopcw - 1, loopch, '@', '*');
      changevalue(b, hassprd, loopcw - 1, loopch + 1, '@', '*');
      changevalue(b, hassprd, loopcw, loopch - 1, '@', '*');
      changevalue(b, hassprd, loopcw, loopch + 1, '@', '*');
    }

    /* Bottom */
    else if(strcmp(whereinforest(loopcw, loopch, maxwidth, maxheight), "B") == 0){

      changevalue(b, hassprd, loopcw - 1, loopch - 1, '@', '*');
      changevalue(b, hassprd, loopcw + 1, loopch - 1, '@', '*');
      changevalue(b, hassprd, loopcw, loopch - 1, '@', '*');
      changevalue(b, hassprd, loopcw - 1, loopch, '@', '*');
      changevalue(b, hassprd, loopcw + 1, loopch, '@', '*');
    }

    /* Central */
    else if(strcmp(whereinforest(loopcw, loopch, maxwidth, maxheight), "C") == 0){

      changevalue(b, hassprd, loopcw - 1, loopch - 1, '@', '*');
      changevalue(b, hassprd, loopcw - 1, loopch, '@', '*');
      changevalue(b, hassprd, loopcw - 1, loopch + 1, '@', '*');
      changevalue(b, hassprd, loopcw, loopch - 1, '@', '*');
      changevalue(b, hassprd, loopcw, loopch, '@', '*');
      changevalue(b, hassprd, loopcw, loopch + 1, '@', '*');
      changevalue(b, hassprd, loopcw + 1, loopch - 1, '@', '*');
      changevalue(b, hassprd, loopcw + 1, loopch, '@', '*');
      changevalue(b, hassprd, loopcw + 1, loopch + 1, '@', '*');
    }

    /* Extinguish original fire */
    setvalue(b, loopcw, loopch, ' ');
  }
}

/* Function to check the neighbouring square 'val' and alter by 'outval' */
void changevalue(int b[WIDTH][HEIGHT], int c[WIDTH][HEIGHT],
                    int loopcw, int loopch, int val, int outval){
  if(whatvalue(b, loopcw, loopch, val) == true){
    setvalue(b, loopcw, loopch, outval);

    /* Need to have another array to check whether it changed this iteration*/
    setvalue(c, loopcw, loopch, '1');
  }
}

/* Function to evaluate value of 2D array is 'z' */
bool whatvalue(int b[WIDTH][HEIGHT], int loopcw, int loopch, int val){

  if(b
    [loopcw][loopch] == val){
    return true;
  }
  return false;
}

/* Function to re-map value of array */
void setvalue(int b[WIDTH][HEIGHT], int loopcw, int loopch, int outval){

  b[loopcw][loopch]= outval;
}


/*Function to evaluate where we are in the forest */
char* whereinforest(int i, int j, int maxwidth, int maxheight){

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

/* Function to establish if a tree grows or lightning hits */
bool treegr_lightn(int prob){
  int x, y;

  x=rand();
  y=x%prob;

  if(y==0){
    return true;
  }
  return false;
}
