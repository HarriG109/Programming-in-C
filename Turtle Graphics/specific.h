/*Specify max size of strings possible*/
#define MAXSTR 100

/*Specify max words*/
#define MAXWORDS 100

/*Specify total alpha chars for do loop*/
#define ALPHA 26

/*Specify escape number (Must be negative)*/
#define ESCPNUM -1

typedef enum bool {false, true} bool;
/*Create type tracker for varnum*/
typedef enum type {neither, num, chr} type;

/*Create structure containing a pointer to the data and a pointer to the key*/
typedef struct instr{
  /* Insturctions */
  char txt[MAXWORDS][MAXSTR];
  /* Word location we're at within instruction*/
  int loc;
  /* Count of do loops */
  int do_ct;
}instr;

/*Structure for do loop*/
typedef struct do_loop{
  /* Letter store */
  char letter[ALPHA];
  /* Current and end values of each letter, NOTE: Only needed for interpreter*/
  int current[ALPHA];
  int end[ALPHA];
  /* Count of each letter in array*/
  int letct;
}do_loop;

/*Structure for SDL drawing*/
typedef struct coord{
  int x;
  int y;
  int angle;
  char* dir;
}coord;
