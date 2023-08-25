/**
 * Simple application to call `java -jar` with given parameters.
 * 
 * Author: vinceynhz
 * https://github.com/vinceynhz/procrun-springboot-exe
 * 
 * This code is provided without guarantees. You're responsible for
 * its usage and any possible effects it may have on your environment.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    return -1;
  }
  char command[1024];
  memset(&command, 0x00, sizeof(command));
  strcat(command, "java -jar");
  for (int index=1; index < argc; index++) {
    strcat(command, " ");
    strcat(command, argv[index]);
  }
  printf("%s\n", command);
  system(command);
  return 0;
}
