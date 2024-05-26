#include <stdio.h>
#include <unistd.h>

int main() {
  printf("Executing ls -a:\n");

  // Execute ls -a
  char *args[] = {"/bin/ls", "-a", NULL};
  execve("/bin/ls", args, NULL);

  // If execve fails
  printf("Error: execve failed\n");

  return 0;
}
