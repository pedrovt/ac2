// --------------------------------
// Guião 1, Ex 5
// Arquitectura de Computadores II
// --------------------------------
// Programa 5 – teste do system call "readStr()" e manipulação de strings

#define STR_MAX_SIZE 20

char *strcat(char *, char *);
char *strcpy(char *, char *);
int strlen(char *);

int main(void) {
  static char str1[STR_MAX_SIZE + 1];
  static char str2[STR_MAX_SIZE + 1];
  static char str3[2 * STR_MAX_SIZE + 1];

  printStr("Introduza 2 strings: ");
  readStr( str1, STR_MAX_SIZE );
  putChar('\n');
  readStr( str2, STR_MAX_SIZE );

  printStr("Resultados:\n");
  printInt( strlen(str1), 10 );     // Length of string 1
  printInt( strlen(str2), 10 );     // Length of string 2

  strcpy(str3, str1);
  printStr( strcat(str3, str2) );   // String 1 + String 2
  printInt10( strcmp(str1, str2) ); // Prints <0, >0, =0 if str 1 is <, >, = than str 2 alphabetically

  return 0;
}


// String length
//
int strlen(char *s)
{
  int len;
  for(len = 0; *s != 0; len++, s++);
  return len;
}

// String concatenate
//
char *strcat(char *dst, char *src)
{
  char *rp = dst;
  for(; *dst != 0; dst++);
  strcpy(dst, src);
  return rp;
}

// String copy
//
char *strcpy(char *dst, char *src)
{
  char *rp = dst;
  for(; (*dst = *src) != 0; dst++, src++);
  return rp;
}

// String compare (alphabetically).
// Returned value is:
// < 0 string "s1" is less than string "s2"
// = 0 string "s1" is equal to string "s2"
// > 0 string "s1" is greater than string "s2"
int strcmp(char *s1, char *s2)
{
  for(; (*s1 == *s2) && (*s1 != 0); s1++, s2++);
  return(*s1 - *s2);
}
