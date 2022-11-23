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
	//Break up path into array of names
	char **path = parse_command(tokens[2], "/");
	//Check that path is valid and get block of parent directory
	int parentBlock = getBlock(path);

	//Get index of last item in path
	int lastIndex = 0;
  while (path[lastIndex] != NULL) {
    lastIndex++;
  }
	lastIndex -= 1;
	//Get name of file
  char name[1];
  strncpy(name, path[lastIndex], 1);

	//Check for first free spot in directory to put file
  int parDirIndex = findFreeSpot(currentDrive->block[parentBlock][2], 8) + 3;

	//Set the correct index of directory to file name
	currentDrive->block[parentBlock][parDirIndex] = name[0];

	//Save drive to file
	close(currentFile);
	currentFile = open(currentName, O_RDWR);
	saveDriveToFile();
}

void list_contents(char **tokens) {
	int parentBlock = 0;
	int currentByte = 3;
	//If not root, follow path till you make it to directory
	if(tokens[1] != NULL) {
		char **path = parse_command(tokens[1], "/");
		int parentBlock = getBlock(path);
		printf("parentBlock: %d\n", parentBlock);
	
		//Get index of last item in path
		int lastIndex = 0;
	  while (path[lastIndex] != NULL) {
	    lastIndex++;
	  }
		lastIndex -= 1;
	
		//Get name of file/directory
	  char name[1];
	  strncpy(name, path[lastIndex], 1);
		printf("fileName: %c\n", name[0]);
	
		//Check that last name in path is valid directory
		if(strlen(path[lastIndex]) > 1 || name[0] < 'A' || name[0] > 'Z') {
			printf("Invalid path");
			return;
		}
	
		currentByte = 3;
		// Loop thru bytes 3-10 looking for directory name
		while (name[0] != currentDrive->block[parentBlock][currentByte] &&
				 currentByte < 11) {
			currentByte++;
		}
		// If went through entire list without finding, display error
		if (currentByte > 10) {
			printf("Directory to remove not found");
			return;
		}		
	
		int currentBlock = 0;
		// Get block number of directory
		int isEven = currentByte % 2;
		currentByte = currentByte / 2 + 11;
		if (!isEven) { //TODO: CHANGED CURRENTBLOCK TO PARENTBLOCK
			int bl = currentDrive->block[parentBlock][currentByte];
			bl = bl / 10;
			currentBlock = currentDrive->block[parentBlock][currentByte] / 10;
		} else {
			int bl = currentDrive->block[parentBlock][currentByte];
			bl = bl % 10;
			currentBlock = currentDrive->block[parentBlock][currentByte] % 10;
		}
	}
	//Loop through directory and display all entries that arent set to 0
	currentByte = 3;
	while (currentByte < 11) {
		if (currentDrive->block[parentBlock][currentByte] != 0) {
			printf("%c ", currentDrive->block[parentBlock][currentByte]);
		}
		currentByte++;
	}
}

Drive *open_file(char **tokens) {
  // close the TFS-file currently in use, if any
  if (currentFile != -1) {
    close(currentFile);
    currentFile = open(currentName, O_RDWR);
    saveDriveToFile();
    close(currentFile);
    currentFile = -1;
    currentName = NULL;
  }
  // open an existing TFS-disk file, fails if file DNE
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
  // check file is appropriate size
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
    close(currentFile);
    currentFile = open(currentName, O_RDWR);
    saveDriveToFile();
    close(currentFile);
    currentFile = -1;
    currentName = NULL;
  }

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
    // Every pathname must have length of 1
    // And be valid uppercase letter
    if (strlen(currentName) > 1 || currentName[0] < 'A' ||
        currentName[0] > 'Z') {
      printf("Invalid path name.");
      return -1;
    }

		int index = 0;
    // Loop thru bytes 3-10 looking for name
    while (currentName[index] != currentDrive->block[currentBlock][currentByte] &&
           currentByte < 11) {
      printf("ln 288");
      fflush(stdout);
      currentByte++;
      // If went through entire list without finding, display error
      if (currentByte > 10) {
        printf("Directory %c not found in path", currentName[0]);
        return -1;
      }
			index++;
    }

    // Get block number of directory
    int isEven = currentByte % 2;
    currentByte = currentByte / 2 + 11;
    if (!isEven) {
      int bl = currentDrive->block[currentBlock][currentByte];
      bl = bl / 10;
      currentBlock = currentDrive->block[currentBlock][currentByte] / 10;
      // memcpy(&bitmap, d->block[0][currentByte], 4);
    } else {
      int bl = currentDrive->block[currentBlock][currentByte];
      bl = bl % 10;
      currentBlock = currentDrive->block[currentBlock][currentByte] % 10;
      // memcpy(&bitmap, d->block[0][currentByte], 8);
      // mask = mask >> 4;
    }
    // TODO: what if isn't used anymore?

    // currentBlock = bitmap & mask;
    currentByte = 3;
    currNameIndex++;
  }
	return currentBlock;
}

void makeDirectory(char **tokens) {
  // confirm there is a free block in the root freespace bitmap
  // TODO: this prob doesn't work
	
  // if (currentDrive->block[0][0] > 256) {
  //   printf("No blocks remaining.");
  //   return;
  // }

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

	printf("after parDirIndex\n");
	fflush(stdout);
	
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

	//Get index of last item in path
	int lastIndex = 0;
  while (path[lastIndex] != NULL) {
    lastIndex++;
  }
	lastIndex -= 1;
  
  char newDir[1];
  strncpy(newDir, path[lastIndex], 1);
	printf("newDir = %c\n", newDir[0]);
	fflush(stdout);
  currentDrive->block[parentBlock][parDirIndex] = path[lastIndex][0];
	
  // Update block pointer
	parDirIndex -= 3;
  int isEven = parDirIndex % 2;
	
  if (!isEven) {		
		//This math is so it will go in the proper block pointer spot at the end
		int newIndex = parDirIndex;
		newIndex = (8-newIndex) /2;
		newIndex = 16 - newIndex;
		//Do the math to see what number the byte should display
    int r = (currentDrive->block[parentBlock][newIndex] / 10) * 10;
		r += blockIndex;		
		//Intended to set char b to the char of a specific ascii value, but it only works sometimes, i dunno why.
		char b = NULL;
		b += r;
    currentDrive->block[parentBlock][newIndex] = b;
  } else {
		int newIndex = parDirIndex;
		newIndex = (8-newIndex) /2;
		newIndex = 15 - newIndex;
		
    int r = currentDrive->block[parentBlock][newIndex];
		r = r % 10;
		r += blockIndex * 10;
		char b = NULL;
		b += r;
    currentDrive->block[parentBlock][newIndex] = b;
  }
  // initialize the bitmap and parent pointer of the new directory block
  // ^It was mentioned that we don't need to do this last step

	//Save drive to file
	close(currentFile);
	currentFile = open(currentName, O_RDWR);
	saveDriveToFile();
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

void removeFromTFS(char **tokens) {
  char **path = parse_command(tokens[1], "/");
	int parentBlock = getBlock(path);

	//Get index of last item in path
	int lastIndex = 0;
  while (path[lastIndex] != NULL) {
    lastIndex++;
  }
	lastIndex -= 1;

	//Get name of file/directory
  char name[1];
  strncpy(name, path[lastIndex], 1);

	//Check name length is valid
	if(strlen(path[lastIndex]) > 1) {
		printf("Item to delete has invalid name length.");
	}
		
	//Check whether it's a file or directory, if neither throw error
	int currentBlock = 0;
	int currentByte = 3;
	if (name[0] > 'A' && name[0] < 'Z') {
    // Loop thru bytes 3-10 looking for name
    while (name[0] != currentDrive->block[parentBlock][currentByte] &&
           currentByte < 11) {
      currentByte++;
			}
      // If went through entire list without finding, display error
      if (currentByte > 10) {
        printf("Directory to remove not found");
        return;
      }		
			int byteNumberInParent = currentByte;
		
		// Get block number of directory
    int isEven = currentByte % 2;
    currentByte = currentByte / 2 + 11;
    if (!isEven) {
      int bl = currentDrive->block[currentBlock][currentByte];
      bl = bl / 10;
      currentBlock = currentDrive->block[currentBlock][currentByte] / 10;
    } else {
      int bl = currentDrive->block[currentBlock][currentByte];
      bl = bl % 10;
      currentBlock = currentDrive->block[currentBlock][currentByte] % 10;
    }

		//Make sure no contents are in the directory
		if(currentDrive->block[currentBlock][2] != 0) {
			printf("Can't delete directory, there are elements inside");
			return;
		} else {
			//Update parent directory bitmap
			currentDrive->block[parentBlock][2] &= ~(1 << byteNumberInParent);
			//Update freespace bitmap			
			currentDrive->block[0][2] &= ~(1 << currentBlock);
		}
	} else if (name[0] > 'a' && name[0] < 'z') {
		//Sort through directory until file is found
		while (name[0] != currentDrive->block[parentBlock][currentByte] &&
           currentByte < 11) {
      currentByte++;
			}
      // If went through entire list without finding, display error
      if (currentByte > 10) {
        printf("Directory to remove not found");
        return;
      } else {
				//Remove file, set to 000000000
				currentDrive->block[parentBlock][currentByte] = 0;
				//Update directory bitmap
				currentDrive->block[parentBlock][2] &= ~(1 << currentByte);
			}
	} else {
		printf("Entry to delete does not have a valid name");
		return;
	}
	
	//Save drive to file
	close(currentFile);
	currentFile = open(currentName, O_RDWR);
	saveDriveToFile();
	
	return;
}