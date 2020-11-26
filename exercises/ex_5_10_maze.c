#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define MAXSTR 10000
#define MAXW 20
#define MAXH 20

typedef enum bool {false, true} bool;

void mazesize(char* a, int* size);
void createmaze(char* file, char a[MAXW][MAXH], int width);
void printarr(char b[MAXW][MAXH], int height, int width);
void cpyoneDtwoDarr(int line, int width, char a[MAXW][MAXH], char* z);
void setarray(char b[MAXW][MAXH], char populate);
void findentrance(char a[MAXW][MAXH], int height);
bool saferead(int a, int b);
bool takestep(char a[MAXW][MAXH], int wp, int hp);
void gothrmaze(char a[MAXW][MAXH], int maxw, int maxh);
int countsteps(char a[MAXW][MAXH], int maxw, int maxh);
bool didyouescape(char a[MAXW][MAXH], int maxw, int maxh);

int main(int argc, char **argv){
  int ms[2];
  char m[MAXW][MAXH];
  int i, loops;

  setarray(m, ' ');

  if( argc==2 ){
    mazesize(argv[1], ms);
    printf("%d %d", ms[0], ms[1]);

    createmaze(argv[1], m, ms[0]);
    printarr(m, ms[0]+1, ms[1]+1);
    /*findentrance(m, ms[1]);
		printarr(m, ms[0]+1, ms[1]+1);

    loops=countsteps(m, ms[0], ms[1]);
    for(i=0; i< loops; i++){
      gothrmaze(m, ms[0], ms[1]);
    }
    if(didyouescape(m, ms[0]-1, ms[1]) == true){
      printf("You escaped!!!\n");
    }
    else{
      printf("No Escape, Mwahahahaaaaaaa!!!\n");
    }*/
  }

  else{
    printf("ERROR: Incorrect usage, try e.g. %s filename\n", argv[0]);
  }

  return 0;
}

bool didyouescape(char a[MAXW][MAXH], int maxw, int maxh){
  int j, count=0;

  for(j=1; j<=maxh; j++){
    if(a[maxw][j] == '.'){
      count++;
    }
  }
  if(count>0){
    return true;
  }
  else{
    return false;
  }
}

int countsteps(char a[MAXW][MAXH], int maxw, int maxh){
  int i, j, count=0;

  for(i=0; i<maxw; i++){
    for(j=1; j<maxh; j++){
      if(a[i][j] == ' '){
        count++;
      }
    }
  }
  return count;
}

void gothrmaze(char a[MAXW][MAXH], int maxw, int maxh){
  int i, j;

  for(i=0; i<= maxw; i++){
    for(j=0; j<= maxh; j++){
      if(takestep(a, i, j) == true){
        a[i][j] = '.';
        i=maxw;
        j=maxh;
      }
    }
  }
  printarr(a, maxw+1, maxh+1);
}

bool takestep(char a[MAXW][MAXH], int wp, int hp){
	int x, y;

	for(x=wp-1; x<=wp+1; x++){
		for(y=hp-1; y<=hp+1; y++){

			if(saferead(x, y) == true){
				if(a[x][y]== '.' && a[wp][hp] == ' ' && (x == wp || y == hp)){
					return true;
				}
			}
		}
  }
  return false;
}

void findentrance(char a[MAXW][MAXH], int height){
	int i;

	for(i=0; i<height; i++){
		/* Don't assess if firest line which is blank for some reason */
		if(saferead(0, i-1) == true){
	    if(a[0][i] == ' '){
	       a[0][i] = '.';
	    }
	  }
	}
}

void createmaze(char* file, char a[MAXW][MAXH], int width){
  char z[MAXSTR];
  int finished = 0;
  FILE *fp;
  int j=0;

  fp=fopen(file, "r");
  if(fp == NULL){
    exit(EXIT_FAILURE);
  }
  while(finished == false){
     if(fgets(z, MAXW, fp) == NULL){
       finished = true;
     }
     j++;

     if(j > 1 && j < MAXH){
       cpyoneDtwoDarr(j-1, width, a, z);
     }
  }
  fclose(fp);
 }

void cpyoneDtwoDarr(int line, int width, char a[MAXW][MAXH], char* z){
  int i=0;

  while(i<width){
    printf("%d %c\n", i, z[i]);
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

void setarray(char b[MAXW][MAXH], char populate){
  int i, j;

  for(j=0; j<MAXH; j++){
    for(i=0; i<MAXW; i++){

      b[i][j]=populate;
    }
  }
}

void printarr(char b[MAXW][MAXH], int height, int width){
  int i, j;

  for(j=0; j<width; j++){
    for(i=0; i<height; i++){


        printf("%c", b[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

bool saferead(int a, int b){
  if(a<0 || b<0 || a >= MAXW || b >= MAXH){
     return false;
  }
  return true;
}
