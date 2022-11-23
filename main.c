#include "ui.h"
#include <stdio.h>

int main(void) {
  // TODO: Optional parameter for file name to open on a.out
  Drive *d = newDrive();
  int exit = 0;
  printf("Virtual file system\n");
  printf(":> ");
  while (!exit) {
    size_t size = 256;
    // TODO: create or open must be called before any other methods that change
    // data
    char *command = malloc(size);
    getline(&command, &size, stdin);
    char **tokens = parse_command(command, " \t\n\v\f\r");
    exit = !(strcmp(tokens[0], "exit"));
    if (!(strcmp(tokens[0], "import")))
      import_file(tokens);
    else if (!strcmp(tokens[0], "ls"))
      list_contents(tokens);
    else if (!strcmp(tokens[0], "display")) {
      printf("%s", displayDrive(d));
      for (int i = 0; i < 16; i++) {
        printf("block %d is used: %d\n", i, isUsed(d, i));
      }
    } else if (!strcmp(tokens[0], "open"))
      d = open_file(tokens);
    else if (!strcmp(tokens[0], "create"))
      d = create(tokens);
    else if (!strcmp(tokens[0], "mkdir"))
      makeDirectory(tokens);
		else if (!strcmp(tokens[0], "rm"))
			removeFromTFS(tokens);
    else if (exit) {
      return (0);
    }

    if (!exit) {
      printf("\n:> ");
    }
  }
  printf("Program terminated.\n");
}