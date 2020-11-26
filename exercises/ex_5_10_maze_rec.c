#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define MAXSTR 10000
#define MAXH 20
#define MAXW 20

typedef enum bool {false, true} bool;

void mazesize(char* a, int* size);
void createmaze(char* file, char a[MAXH][MAXW], int width);
void printarr(char b[MAXH][MAXW], int height, int width);
void cpyoneDtwoDarr(int line, int width, char a[MAXH][MAXW], char* z);
void setarray(char b[MAXH][MAXW], char populate);
int findentrance(char a[MAXH][MAXW], int height);
bool saferead(int a, int b);
void explore(int x, int y, char mz[MAXH][MAXW], int end);

int main(int argc, char **argv){
  int ms[2];
  char m[MAXH][MAXW];

  setarray(m, ' ');

  if( argc==2 ){
    mazesize(argv[1], ms);

    createmaze(argv[1], m, ms[0]);
    printarr(m, ms[0]+1, ms[1]+1);
    findentrance(m, ms[1]);
    explore(0, findentrance(m, ms[1]), m, ms[1]);
    printarr(m, ms[0]+1, ms[1]+1);
  }

  else{
    printf("ERROR: Incorrect usage, try e.g. %s filename\n", argv[0]);
  }

  return 0;
}

/*From Neills lecture on recursion and tweaked*/
void explore(int x, int y, char mz[MAXH][MAXW], int end){

  if(saferead(x, y) == true && x <= end){
    printf("%d %d %c", x, y, mz[x][y]);
    if(mz[x][y] == ' '){
      mz[x][y] = '.';
    }
  }

  explore(x, y+1, mz, end);
  explore(x, y-1, mz, end);
  explore(x+1, y, mz, end);
  explore(x-1, y, mz, end);
}

/*Gives y co-ordinates of the entrance*/
int findentrance(char a[MAXH][MAXW], int height){
	int i;

	for(i=0; i<height; i++){
		/* Don't assess if first line, which is blank for some reason */
		if(saferead(0, i-1) == true){
	    if(a[0][i] == ' '){
	       return i;
	    }
	  }
	}
  return 0;
}

void createmaze(char* file, char a[MAXH][MAXW], int width){
  char z[MAXSTR];
  int finished = 0;
  FILE *fp;
  int j=0;

  fp=fopen(file, "r");
  if(fp == NULL){
    exit(EXIT_FAILURE);
  }
  while(finished == false){
     if(fgets(z, MAXH, fp) == NULL){
       finished = true;
     }
     j++;

     if(j > 1){
       cpyoneDtwoDarr(j-1, width, a, z);
     }
  }
  fclose(fp);
 }

void cpyoneDtwoDarr(int line, int width, char a[MAXH][MAXW], char* z){
  int i=0;

  while(i<width){
    a[i][line]= z[i];
    i++;
  }
}

void mazesize(char* file, int* size){
  char z[MAXSTR];

  int finished = 0;
  FILE *fp;
  int p, q, i;

  fp=fopen(file, "r");
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
     }
   }
   fclose(fp);

   for(i=0; i<2; i++){
     if(i==0){
       size[i]=p;
     }
     else if(i==1){
       size[i]=q;
     }
   }
}

void setarray(char b[MAXH][MAXW], char populate){
  int i, j;

  for(j=0; j<MAXW; j++){
    for(i=0; i<MAXH; i++){

      b[i][j]=populate;
    }
  }
}

void printarr(char b[MAXH][MAXW], int height, int width){
  int i, j;

  for(j=0; j<height; j++){
    for(i=0; i<width; i++){


        printf("%c", b[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

bool saferead(int a, int b){
  if(a<0 || b<0 || a >= MAXH || b >= MAXW){
     return false;
  }
  return true;
}
