#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
  unsigned char *block[16];
} Drive;

Drive *newDrive();
char *displayDrive(Drive *);
int isUsed(Drive *d, int pos);
unsigned char *dump(Drive *);
