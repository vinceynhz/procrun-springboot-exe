/**
 * Simple application to call `taskkill /F /PID` with given parameters.
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
  FILE * pidfile; 
  char pid[50];
  char command[1024];
  
  memset(&command, 0x00, sizeof(command));
  memset(&pid, 0x00, sizeof(pid));
  
  pidfile = fopen(argv[1], "r");
  fgets(pid, sizeof(pid), pidfile);
  fclose(pidfile);
  
  sprintf(command, "taskkill /F /PID %s", pid);
  
  printf("%s\n", command);
  system(command);
  return 0;
}
