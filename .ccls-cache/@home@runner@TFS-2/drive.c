#include "drive.h"

unsigned char *dump(Drive *d) {
  unsigned char *ret = malloc(256);
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      ret[16 * i + j] = d->block[i][j];
    }
  }
  return ret;
}

Drive *newDrive() {
  Drive *d = malloc(sizeof(Drive));
  for (int i = 0; i < 16; i++) {
    d->block[i] = malloc(16);
  }
  d->block[0][0] = 1; // 0000 0001
  d->block[0][1] = 0; // 0000 0000
  return d;
}

int isUsed(Drive *d, int pos) {
  unsigned short bitmap = 0;
  if (pos >= 0 && pos <= 16)
    memcpy(&bitmap, d->block[0], 2);
  unsigned short mask = 1;
  if (pos >= 0 && pos <= 16)
    mask = mask << pos;
  else
    mask = 0;
  return !((mask & bitmap) == 0);
}

char *displayDrive(Drive *d) {
  const int RETSIZE = 1024;
  char *temp = malloc(RETSIZE);
  char *ret = malloc(RETSIZE);
  ret[0] = '\0';
  strncat(ret, "   ", 1024);
  for (int i = 0; i < 16; i++) {
    sprintf(temp, "  %x", i);
    strncat(ret, temp, RETSIZE);
  }
  strncat(ret, "\n", RETSIZE);

  for (int row = 0; row < 16; row++) {
    sprintf(temp, "%x: ", row);
    strncat(ret, temp, RETSIZE);
    for (int col = 0; col < 16; col++) {
      sprintf(temp, "%3.2x", d->block[row][col]);
      strncat(ret, temp, RETSIZE);
    }
    strncat(ret, "\n", RETSIZE);
  }
  return ret;
}
