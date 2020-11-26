 #include <stdio.h>
#include <assert.h>

#define MAXLINE 5

int romanToArabic( char *roman );
char* transformString(char s[], int size);
int returnnumb( char *roman, int y, int x, int lpctr);

void test();

int main(int argc, char **argv){

  if( argc==2 ){
    /*printf("The roman numeral %s is equal to %d\n", \*/
    /*argv[1], romanToArabic(argv[1]));*/
    romanToArabic(argv[1]);
  }
  else{
    printf("ERROR: Incorrect usage, try e.g. %s XXI\n", argv[0]);
  }
return 0;
}

void test(){

}

int romanToArabic( char *roman ){
  int i;
  int total=0;

  for(i=0; roman[i] != '\0'; i++){

      /* Double letters */
      if(roman[i] == 'I'){
        if(roman[i+1]== 'V'){
          total=total+returnnumb(roman, 'V', 4, i+1);
          i++;
          printf("%d\n", 4);
        }
        else if(roman[i+1] == 'X'){
          total=total+returnnumb(roman, 'X', 9, i+1);
          i++;
          printf("%d\n", 9);
        }
        else{
          total=total+returnnumb(roman, 'I', 1, i);
          printf("%d\n", 1);
        }
      }
      else if(roman[i] == 'X'){
        if(roman[i+1] == 'L'){
          total=total+returnnumb(roman, 'L', 40, i+1);
          i++;
          printf("%d\n", 40);
        }
        else if(roman[i+1] == 'C'){
          total=total+returnnumb(roman, 'C', 90, i+1);
          i++;
          printf("%d\n", 90);
        }
        else{
          total=total+returnnumb(roman, 'X', 10, i);
          printf("%d\n", 10);
        }
      }
      else if(roman[i] == 'C'){
        if(roman[i+1] == 'D'){
          total=total+returnnumb(roman, 'D', 400, i+1);
          printf("%d\n", 400);
          i++;
        }
        else if(roman[i+1] == 'M'){
          total=total+returnnumb(roman, 'M', 900, i+1);
          printf("%d\n", 900);
          i++;
        }
        else{
          total=total+returnnumb(roman, 'C', 100, i);
          printf("%d\n", 100);
        }
      }

      /* Singles */
      else if(roman[i] == 'V'){
         total=total+returnnumb(roman, 'V', 5, i);
         printf("%d\n", 5);
      }
      else if(roman[i] == 'L'){
        total=total+returnnumb(roman, 'L', 50, i);
        printf("%d\n", 50);
      }
      else if(roman[i] == 'D'){
        total=total+returnnumb(roman, 'D', 500, i);
        printf("%d\n", 500);
      }
      else if(roman[i] == 'M'){
        total=total+returnnumb(roman, 'M', 1000, i);
        printf("%d\n", 1000);
      }
  }

  printf("%d\n", total);

 return 0;

}

int returnnumb( char *roman, int y, int x, int lpctr){

  if(roman[lpctr] == y){
    return x;
  }
  return 0;
}
