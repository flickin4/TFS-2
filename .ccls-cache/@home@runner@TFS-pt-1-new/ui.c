#include "ui.h"

int currentFile = -1;
Drive *currentDrive;
char *currentName;

char **parse_command(char *command, char *whitespace) {
  char *token;

  char *cmd;
  cmd = malloc(strlen(command));
  strncpy(cmd, command, strlen(command));

  // count tokens, start with first
  int count = 1;
  token = strtok(command, whitespace);

  /* walk through other tokens */
  while (token != NULL) {
    token = strtok(NULL, whitespace);
    count++;
  }
  char **ret = malloc((count + 1) * sizeof(char *));
  ret[count] = malloc(1);
  ret[count] = NULL;

  // read first token into ret
  token = strtok(cmd, whitespace);
  ret[0] = malloc(strlen(token));
  strncpy(ret[0], token, strlen(token));
  token = strtok(NULL, whitespace);
  count = 1;

  /* walk through other tokens */
  while (token != NULL) {
    ret[count] = malloc(strlen(token));
    strncpy(ret[count], token, 20);
    count++;
    token = strtok(NULL, whitespace);
  }

  return ret;
}

void import_file(char **tokens) {
  // bullshitting this for now and pretending 1 is directory name and 2 is file
  // name.

  // Copy a file stored in the regular file system to the current TFS-disk. For
  // part 1, LP is ignored, and an empty file is created at location tp.

  // steps

  // find first free block pointer in the directory from the bitmap
  // update the bitmap, write the file name in the correct byte
}

void list_contents(char **tokens) {
  // maybe do strtok to break up names for path.
  // char *names;
  // char DirName = strtok()

  // list the contents of the given directory of the TFS-disk
  // steps:
  // confirm the path tp points to a valid directory, if not report error
  printf("inside list_contents");
  fflush(stdout);
  // printf(tokens[1]);

  // int pathSize = sizeof(tokens[2]);
  // printf("inside list_contents2");
  // fflush(stdout);
  // printf("%d\n", pathSize);
  // fflush(stdout);

  // char idk[15];
  // strncpy(&idk, tokens[2], 3);
  // printf("idk");
  // 	fflush(stdout);
  // printf("%s", idk);
  for (int i = 0; i < 10; i += 2) {
    printf("inside for");
    fflush(stdout);
    char currentDir = tokens[2][i];
    printf("idk");
    fflush(stdout);
    printf("%c\n", currentDir);
    fflush(stdout);
    if (currentDir <= 'Z' && currentDir >= 'A') {
      printf("%c\n", currentDir);
    } else { // Add else-if to check if file name for import_file
      printf("Directory path invalid\n");
      return;
    }
  }
  // int currentIndex = 0;
  // for (int i = 0; i < strnlen(names, 8); i++) {
  //   int entryIndex = 3;
  //   char currentEntry = currentDrive->block[currentIndex][entryIndex];
  //   while (entryIndex < 11 && currentEntry != names[i]) {
  //     char currentEntry = currentDrive->block[currentIndex][entryIndex];
  //   }
  //   if (currentEntry !=) {
  //   }
  // }

  // use the directory's bitmap to determine which entries are valid
  // print out the one character names of the valid entries

  // int dirIndex = 0;
  // int entryIndex = 3;

  // char currentEntry = currentDrive->block[dirIndex][entryIndex];
  // while(entryIndex = )
}

Drive *open_file(char **tokens) {
  // close the TFS-file currently in use, if any
  if (currentFile != -1) {
    printf("For rewrite, currentName = %s\n", currentName);
    fflush(stdout);
    close(currentFile);
    currentFile = open(currentName, O_RDWR);
    printf("%s\n", currentName);
    fflush(stdout);
    printf("%d\n", currentFile);
    fflush(stdout);
    saveDriveToFile();
    close(currentFile);
    currentFile = -1;
    currentName = NULL;
  }
  // open an existing TFS-disk file, fails if file DNE
  // printf("before adding .bin");
  // fflush(stdout);
  // const char *name = tokens[1];
  // const char *extension = ".bin";
  // char *fileName = malloc(strlen(name) + 1 + 4);
  // strcpy(fileName, name);
  // strcat(fileName, extension);
  // printf("%s\n", fileName);
  // fflush(stdout);
  // currentFile = open(fileName, O_RDWR);
  // printf("%s", fileName);
  // fflush(stdout);
  char *fileName = tokens[1];
  currentFile = open(fileName, O_RDWR);

  if (currentFile == -1) {
    printf("Error, file invalid.\n");
    fflush(stdout);
    return currentDrive;
  }
  currentName = fileName;
  printf("currentName = %s\n", fileName);
  fflush(stdout);

  // make sure size of file is valid
  struct stat myInode;
  int ret = stat(fileName, &myInode);
  int fileSize = myInode.st_size;
  // TODO: check size when size is properly set for files
  // if (fileSize == 256) {
  currentDrive = newDrive();
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      unsigned char b[1];
      read(currentFile, b, 1);
      currentDrive->block[i][j] =
          (int)b[0]; // TODO: should only cast if it's actually an int
    }
  }
  printf("%s", displayDrive(currentDrive));
  for (int i = 0; i < 16; i++) {
    printf("block %d is used: %d\n", i, isUsed(currentDrive, i));
  }
  // } else {
  //   printf("Error, file size is invalid.\n");
  // }
  return currentDrive;
}

Drive *create(char **tokens) {
  // close the TFS-file currently in use, if any
  if (currentFile != -1) {
    printf("For rewrite, currentName = %s\n", currentName);
    fflush(stdout);
    close(currentFile);
    currentFile = open(currentName, O_RDWR);
    printf("%s\n", currentName);
    fflush(stdout);
    printf("%d\n", currentFile);
    fflush(stdout);
    saveDriveToFile();
    close(currentFile);
    currentFile = -1;
    currentName = NULL;
  }

  // if file exists, display error
  // printf("before adding .bin");
  // fflush(stdout);
  // const char *name = tokens[1];
  // const char *extension = ".bin";
  // char *fileName = malloc(strlen(name) + 1 + 4);
  // strcpy(fileName, name);
  // strcat(fileName, extension);
  // printf("%s\n", fileName);
  // fflush(stdout);
  // currentFile = open(fileName, O_RDONLY);
  // printf("file opened\n");
  // fflush(stdout);
  char *fileName = tokens[1];
  currentFile = open(fileName, O_RDONLY);

  if (currentFile != -1) {
    printf("File already exists.\n");
    fflush(stdout);
    close(currentFile);
    currentFile = -1;
    return currentDrive;
  }

  // Create new file with specified name
  currentFile = open(fileName, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
  if (currentFile == -1) {
    printf("Failed to create new file.\n");
    fflush(stdout);
    return currentDrive;
  }
  currentName = fileName;
  printf("currentName = %s\n", currentName);
  fflush(stdout);

  // create empty disk in memory
  currentDrive = newDrive();
  saveDriveToFile();

  return currentDrive;
}

void saveDriveToFile() {
  write(currentFile, dump(currentDrive), 700);

  // // save it to the open file
  // unsigned char *ret = malloc(256);

  // strncpy(ret, dump(currentDrive), 256);

  // const int RETSIZE = 700;
  // unsigned char *temp = malloc(RETSIZE);
  // unsigned char *fileOutput = malloc(RETSIZE);
  // for (int i = 0; i < 256; i++) {
  //   sprintf(temp, "%x", ret[i]);
  //   strncat(fileOutput, temp, RETSIZE);
  // }

  // write(currentFile, fileOutput, RETSIZE);
}

int getBlock(char **path) {
  // CHECK IF PATH IS VALID!
  if (sizeof(path) > 17) { // TODO: this prob wont work
    printf("Path is out of range.");
    return -1;
  }

  int currNameIndex = 0;
  char *currentName = path[0];
  int currentBlock = 0;
  int currentByte = 3;

  // Loop through names in path
  // Last valid index should be name of new directory/file
  while (path[currNameIndex + 1] != NULL) {
    printf("ln 276");
    fflush(stdout);
    // Every pathname must have length of 1
    // And be valid uppercase letter
    if (strlen(currentName) > 1 || currentName[0] < 'A' ||
        currentName[0] > 'Z') {
      printf("Invalid path name.");
      return -1;
    }

    // Loop thru bytes 3-10 looking for name
    while (currentName[0] != currentDrive->block[currentBlock][currentByte] &&
           currentByte < 11) {
      printf("ln 288");
      fflush(stdout);
      currentByte++;
      // If went through entire list without finding, display error
      if (currentByte > 10) {
        printf("Directory %c not found in path", currentName[0]);
        return -1;
      }
    }

    printf("currentBlock: %d, currentByte: %d", currentBlock, currentByte);
    fflush(stdout);

    // Get block number of directory
    int isEven = currentByte % 2;
    currentByte = currentByte / 2 + 11;
    if (!isEven) {
      printf("currentByte: %d, currentBlock = %d\n", currentByte, currentBlock);
      fflush(stdout);
      // TODO: clean up!
      int bl = currentDrive->block[currentBlock][currentByte];
      printf("bl: %d\n", bl);
      fflush(stdout);
      bl = bl / 10;
      printf("bl: %d\n", bl);
      fflush(stdout);
      currentBlock = currentDrive->block[currentBlock][currentByte] / 10;
      // memcpy(&bitmap, d->block[0][currentByte], 4);
    } else {
      printf("currentByte: %d, currentBlock = %d\n", currentByte, currentBlock);
      fflush(stdout);
      int bl = currentDrive->block[currentBlock][currentByte];
      printf("bl: %d\n", bl);
      fflush(stdout);
      bl = bl % 10;
      printf("bl: %d\n", bl);
      fflush(stdout);
      currentBlock = currentDrive->block[currentBlock][currentByte] % 10;
      // memcpy(&bitmap, d->block[0][currentByte], 8);
      // mask = mask >> 4;
    }
    // TODO: what if isn't used anymore?

    // currentBlock = bitmap & mask;
    currentByte = 3;
    currNameIndex++;
  }
  printf("currentBlock: %d\n", currentBlock);
  return currentBlock;
}

void makeDirectory(char **tokens) {
  // confirm there is a free block in the root freespace bitmap
  // TODO: this prob doesn't work
  if (currentDrive->block[0][0] > 255) {
    printf("No blocks remaining.");
    return;
  }

  // confirm path tp points to valid location for new directory
  char **path = parse_command(tokens[1], "/");
  int parentBlock = getBlock(path);
  if (parentBlock == -1)
    return;

  // use freespace bitmap to find block for new directory
  int blockIndex = 0;
  while (isUsed(currentDrive, blockIndex)) {
    blockIndex++;
  }

  // use the parent directory bitmap to find an entry for tp
  int parDirIndex = findFreeSpot(currentDrive->block[parentBlock][2], 8) + 3;

  if (parDirIndex == -1) {
    printf("No free space to add to parent directory");
    return;
  }

  // update the root freespace bitmap to remove the selected block
  unsigned short bitmap = 0;
  memcpy(&bitmap, currentDrive->block[0], 2);
  unsigned short mask = 1;
  mask = mask << blockIndex;
  currentDrive->block[0][0] |= 1 << blockIndex;

  // update the parent directory bitmap, name, and block pointer: tp
  currentDrive->block[parentBlock][2] |= 1 << (parDirIndex - 3);

  // int *lastIndex = (int *)(&path + 1) - 1;
  // printf("path's last index = %d", *lastIndex);
  // printf("last path in path: %s", path[*lastIndex]);
  // currentDrive->block[parentBlock][parDirIndex] = *path[*lastIndex];
  int *lastIndex = (int *)(&path + 1) - 1;
  char *newDir = malloc(sizeof(char) + 1);
  strncpy(newDir, path[*lastIndex], 1);
  currentDrive->block[parentBlock][parDirIndex] = newDir[0];
  // Update block pointer
  int isEven = parDirIndex % 2;
  parDirIndex = parDirIndex / 2 + 11;
  if (isEven) {
    // First step is incase block pointer exists for old deleted directory
    // int r = 32;
    // unsigned char b = r;
    // currentDrive->block[0][parDirIndex] = b;

    currentDrive->block[0][parDirIndex] =
        (currentDrive->block[0][parDirIndex] / 10) * 10;
    currentDrive->block[0][parDirIndex] += blockIndex;
  } else {
    // int r = 42;
    // unsigned char b = r;
    // currentDrive->block[0][parDirIndex] = b;

    currentDrive->block[0][parDirIndex] =
        currentDrive->block[0][parDirIndex] % 10;
    currentDrive->block[0][parDirIndex] += blockIndex * 10;
  }
  // initialize the bitmap and parent pointer of the new directory block
  // I don't think we are sposed to do this last step?
}

int findFreeSpot(unsigned char bitmap, int bitsize) {
  int index = 0;
  unsigned char mask = bitsize / 8;
  while (((mask & bitmap) != 0) && index < bitsize) {
    index++;
    mask = mask << 1;
  }
  if (index < bitsize) {
    return index;
  } else {
    return -1;
  }
}

void removeFromTFS(char **tokens, Drive *d) {
  // Remove a file or directory from the TFS-disk. Fails if the file or
  // directory does not exist, or if the directory is not empty.

  // steps:
  // confirm tp is valid, else error
  // update parent directory bitmap
  // update freespace bitmap
}