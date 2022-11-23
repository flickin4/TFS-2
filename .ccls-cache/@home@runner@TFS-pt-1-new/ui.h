#include "drive.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <errno.h>

char **parse_command(char *command, char *whitespace);
void import_file(char **tokens);
void list_contents(char **tokens);
Drive *create(char **tokens);
Drive *open_file(char **tokens);
int findFreeSpot(unsigned char bitmap, int bitsize);
void makeDirectory(char **tokens);
void saveDriveToFile();