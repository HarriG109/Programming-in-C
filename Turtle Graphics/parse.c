#include "specific.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

void get_file(char* file, char a[MAXWORDS][MAXSTR]);
instr* init_instr();
do_loop* init_do();
void _startbrac(instr* a, do_loop* b);
void instr_list(instr* a, do_loop* b);
void instruct(instr* a, do_loop* b);
bool is_number(char *str);
void fd_lt_rt(instr* a, do_loop* b);
void do_func(instr* a, do_loop* b);
void set_func(instr* a, do_loop* b);
void polish(instr* a, do_loop* b);
void op_varnum(instr* a, do_loop* b);
type varnum(instr* a, do_loop* b, char* s);
void check_letter_used(instr* a, do_loop* b);
int check_letter_alloc(char letr, do_loop* b);
void test();

int main(int argc, char **argv){

  /*Test*/
  /*test();*/

  /*Initialize struct*/
  instr* turtle;
  do_loop* loop;
  turtle = init_instr();
  loop = init_do();

  if(argc == 2){
    get_file(argv[1], turtle -> txt);
    _startbrac(turtle, loop);
  }
  else{
    printf("ERROR: Incorrect usage, try e.g. %s filename\n" , argv[0]);
  }
  /*Free structures*/
  if(turtle != NULL){
    free(turtle);
  }
  if(loop != NULL){
    free(loop);
  }

  return 0;
}

/*Function for testing*/
void test(){

  /*Initialize struct*/
  instr* a;
  do_loop* b;
  a = init_instr();
  b = init_do();

  /*Test values arte as expected*/
  assert(a -> txt[0][0] == '\0');
  assert(a -> txt[10][0] == '\0');
  assert(a -> txt[17][0] == '\0');
  assert(a -> loc == 0);
  assert(a -> do_ct == 0);
  assert(b -> letter[0] == '\0');
  assert(b -> letter[13] == '\0');
  assert(b -> letter[25] == '\0');
  assert(b -> current[0] == 0);
  assert(b -> current[13] == 0);
  assert(b -> current[25] == 0);
  assert(b -> end[0] == 0);
  assert(b -> end[13] == 0);
  assert(b -> end[25] == 0);
  assert(b -> letct == 0);

  /******************* Check letter allocation testing ********************/
  b -> letter[0] = 'C';
  b -> letct++;
  b -> letter[1] = 'B';
  b -> letct++;
  b -> letter[2] = 'A';
  b -> letct++;

  assert(check_letter_alloc('A', b) == 2);
  assert(check_letter_alloc('C', b) == 0);
  assert(check_letter_alloc('E', b) == ESCPNUM);
  assert(check_letter_alloc('Z', b) == ESCPNUM);


  /*********************** Varnum function testing ************************/
  /*Check string is rejected*/
  strcpy(a -> txt[a -> loc], "HELLO");
  assert(varnum(a, b, "DO") == neither);
  a -> loc++;

  /*Check numbers flagged ok as num*/
  strcpy(a -> txt[a -> loc], "2");
  assert(varnum(a, b, "DO") == num);
  a -> loc++;

  strcpy(a -> txt[a -> loc], "9844262");
  assert(varnum(a, b, "DO") == num);
  a -> loc++;

  strcpy(a -> txt[a -> loc], "3.1224");
  assert(varnum(a, b, "DO") == num);
  a -> loc++;

  strcpy(a -> txt[a -> loc], "-199");
  assert(varnum(a, b, "DO") == num);
  a -> loc++;

  /*Check single upper cased letter flagged ok as chr,
  NOTE: Must not be an already stored letter for DO*/
  strcpy(a -> txt[a -> loc], "X");
  assert(varnum(a, b, "DO") == chr);
  a -> loc++;

  /*Check lower case letter flagged as not expected*/
  strcpy(a -> txt[a -> loc], "a");
  assert(varnum(a, b, "DO") == neither);
  a -> loc++;

  /*Check special char letter flagged as not expected*/
  strcpy(a -> txt[a -> loc], "*");
  assert(varnum(a, b, "DO") == neither);
  a -> loc++;

  /*Test letter allocation is checked correctly for non-DO loop
  (Allocation done in previous function tests)*/
  strcpy(a -> txt[a -> loc], "A");
  assert(varnum(a, b, "NOT DO") == chr);
  a -> loc++;

  /*Free structures*/
  if(a != NULL){
    free(a);
  }
  if(b != NULL){
    free(b);
  }
}

/*Initialise pointer to commands structure*/
instr* init_instr(){
  instr* a;
  int j;

  a = calloc(1, sizeof(instr));
  if(a == NULL){
    printf("Could not allocate space\n");
    exit(EXIT_FAILURE);
  }
  a -> loc = 0;
  a -> do_ct = 0;
  for(j=0; j<MAXWORDS; j++){
      a -> txt[j][0] = '\0';
  }

  return a;
}

/*Initialize do loop struct*/
do_loop* init_do(){
  do_loop* b;
  int i;

  b = calloc(1, sizeof(do_loop));
  if(b == NULL){
    printf("Could not allocate space\n");
    exit(EXIT_FAILURE);
  }
  for(i=0; i<ALPHA; i++){
      b -> letter[i] = '\0';
      b -> current[i] = 0;
      b -> end[i] = 0;
      b -> letct = 0;
  }

  return b;
}

/*Function to get commands from file*/
void get_file(char* file, char a[MAXWORDS][MAXSTR]){
  FILE *fp;
  int i=0;

  fp=fopen(file, "r");
  if(fp == NULL){
    printf("File will not open\n");
    exit(EXIT_FAILURE);
  }
  while(fscanf(fp, "%s", a[i++]) == 1 && i < MAXWORDS){}
  fclose(fp);
}

/* Function which checks start bracket and continues if fine*/
void _startbrac(instr* a, do_loop* b){

  /*Check for starting bracket */
  if(strcmp(a -> txt[a -> loc], "{") != 0){
    printf("Starting bracket missing\n");
    exit(EXIT_FAILURE);
  }
  else{
    instr_list(a , b);
  }
}

/*Function which recursively calls if commands are expected*/
void instr_list(instr* a, do_loop* b){

  /* Increment to next value*/
  a -> loc++;

  if(strcmp(a -> txt[a -> loc], "}") == 0){
    if(a -> do_ct == 0){
      return;
    }
    else{
      /* Decrement do counter */
      a -> do_ct--;
      instr_list(a, b);
    }
  }
  else{
    instruct(a, b);
    instr_list(a, b);
  }
}

/*Function for each expected instruction*/
void instruct(instr* a, do_loop* b){
  if(strcmp(a -> txt[a -> loc], "FD") == 0){
    fd_lt_rt(a, b);
    return;
  }
  else if(strcmp(a -> txt[a -> loc], "RT") == 0){
    fd_lt_rt(a, b);
    return;
  }
  else if(strcmp(a -> txt[a -> loc], "LT") == 0){
    fd_lt_rt(a, b);
    return;
  }
  else if(strcmp(a -> txt[a -> loc], "DO") == 0){
    do_func(a, b);
    /*Increment do loop counter*/
    a -> do_ct++;
    return;
  }
  else if(strcmp(a -> txt[a -> loc], "SET") == 0){
    set_func(a, b);
    return;
  }
  else{
    printf("Unexpected instruction %s\n", a -> txt[a -> loc]);
    exit(EXIT_FAILURE);
  }
}

/*Function if FD, RT, LT is found*/
void fd_lt_rt(instr* a, do_loop* b){
  /* Increment to next value*/
  a -> loc++;

  /*Check if it's a number or an allocated letter*/
  if(varnum(a, b, "NOT DO") == num || varnum(a, b, "NOT DO") == chr){
    return;
  }
  else{
    printf("Expected number or assigned letter after instruction, stopped at: %s\n"
           , a -> txt[a -> loc]);
    exit(EXIT_FAILURE);
  }
}

/*Function to evaluate do loop*/
void do_func(instr* a, do_loop* b){

  /* Increment to next value*/
  a -> loc++;

  /*Check letter is alphabetical single char*/
  if(isalpha(a -> txt[a -> loc][0]) && a -> txt[a -> loc][1] == '\0'){
    /*Check letter hasn't been used already*/
    check_letter_used(a, b);

    /*Store value in struct*/
    b -> letter[b -> letct] = a -> txt[a -> loc][0];

    /*Increment letter count*/
    b -> letct++;

    /* Increment to next value*/
    a -> loc++;

    /*Check for 'FROM'*/
    if(strcmp(a -> txt[a -> loc], "FROM") == 0){
      /* Increment to next value*/
      a -> loc++;

      /*Check for numeric or single upper cased character*/
      if(varnum(a, b, "DO") != neither){

        /* Increment to next value*/
        a -> loc++;

        /*Check for 'TO'*/
        if(strcmp(a -> txt[a -> loc], "TO") == 0){
          /* Increment to next value*/
          a -> loc++;

          /*Check for numeric or character*/
          if(varnum(a, b, "DO") != neither){
            /* Increment to next value*/
            a -> loc++;

            /*Check for start bracket*/
            if(strcmp(a -> txt[a -> loc], "{") == 0){

              return;
            }
            else{
              printf("Missing starting bracket after do loop: %s\n", a -> txt[a -> loc]);
              exit(EXIT_FAILURE);
            }
          }
          else{
            printf("Missing numeric end value for DO loop: %s\n", a -> txt[a -> loc]);
            exit(EXIT_FAILURE);
          }
        }
        else{
          printf("Missing 'TO' in DO loop: %s\n", a -> txt[a -> loc]);
          exit(EXIT_FAILURE);
        }
      }
      else{
        printf("Missing numeric start value for DO loop: %s\n", a -> txt[a -> loc]);
        exit(EXIT_FAILURE);
      }
    }
    else{
      printf("Missing 'FROM' in DO loop: %s\n", a -> txt[a -> loc]);
      exit(EXIT_FAILURE);
    }
  }
  else{
    printf("Missing single alphabetical char: %s\n", a -> txt[a -> loc]);
    exit(EXIT_FAILURE);
  }
}

/*Function to evaluate set function*/
void set_func(instr* a, do_loop* b){
  /* Increment to next value */
  a -> loc++;

  /*Check letter is alphabetical single char*/
  if(isalpha(a -> txt[a -> loc][0]) && a -> txt[a -> loc][1] == '\0'){
    /*Assign letter into struct*/
    b -> letter[b -> letct] = a -> txt[a -> loc][0];

    /* Increment to next value*/
    a -> loc++;

    /*Check equal sign*/
    if(strcmp(a -> txt[a -> loc], ":=") == 0){
      /*Check polish*/
      polish(a, b);

      /*Increment letter count once confirmed ok*/
      b -> letct++;

      return;
    }
    else{
      printf("Missing equal sign for polish assignment: %s\n", a -> txt[a -> loc]);
      exit(EXIT_FAILURE);
    }
  }
  else{
    printf("Missing single alphebetical char: %s\n", a -> txt[a -> loc]);
    exit(EXIT_FAILURE);
  }
}

/*Function to evaluate polish*/
void polish(instr* a, do_loop* b){
  /* Increment to next value*/
  a -> loc++;

  if(strcmp(a -> txt[a -> loc], ";") == 0){
    return;
  }
  else{
    op_varnum(a, b);
    polish(a, b);
  }
}

/*Function for op or varnum value*/
void op_varnum(instr* a, do_loop* b){

  if(strcmp(a -> txt[a -> loc], "/") == 0){
    return;
  }
  else if(strcmp(a -> txt[a -> loc], "+") == 0){
    return;
  }
  else if(strcmp(a -> txt[a -> loc], "-") == 0){
    return;
  }
  else if(strcmp(a -> txt[a -> loc], "*") == 0){
    return;
  }
  else if(varnum(a, b, "NOT DO") != neither){
      return;
  }
  else{
    printf("Expecting polish sign, number or single assigned letter. Please check %s"
           , a -> txt[a -> loc]);
    exit(EXIT_FAILURE);
  }
}

/*Function to check if varnum (Single letter or number) NOTE: str modifier is used to
alter if function is for DO loop (where the letter shouldn't already be allocated)
or not (where the letter should already be allocated)*/
type varnum(instr* a, do_loop* b, char* s){

  /*Check if it's a number*/
  if(atoi(a -> txt[a -> loc]) != 0){

    return num;
  }
  /*Check if it's a single upper cased letter*/
  else if(isalpha(a -> txt[a -> loc][0]) && a -> txt[a -> loc][1] == '\0'
           && isupper(a -> txt[a -> loc][0])){
    if(strcmp(s, "DO") == 0){
      /*Check if letter isn't already allocated*/
      if(check_letter_alloc(a -> txt[a -> loc][0], b) == ESCPNUM){

        return chr;
      }
    }
    else{
      /*Check if letter is allocated*/
      if(check_letter_alloc(a -> txt[a -> loc][0], b) != ESCPNUM){

        return chr;
      }
    }
    return neither;
  }
  else{
    return neither;
  }
}

/* Function to check for repeated letter in do loop */
void check_letter_used(instr* a, do_loop* b){
  int i;

  if(b -> letct != 0){
    for(i=0; i < b -> letct; i++){
      if(a -> txt[a -> loc][0] == b -> letter[i]){
        printf("Loop letter already allocated, pick a different letter: %c\n"
               , a -> txt[a -> loc][0]);
        exit(EXIT_FAILURE);
      }
    }
  }
}

/* Function to return position of letter in array */
int check_letter_alloc(char letr, do_loop* b){
  int i;

  if(b -> letct == 0){
    return ESCPNUM;
  }
  else{
    for(i=0; i < b -> letct; i++){
      if(b -> letter[i] == letr){
        return i;
      }
    }
    return ESCPNUM;
  }
}
