#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  pid_t pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Failed to fork process\n");
    return 1;
  }
  else if (pid == 0) {
    // Child process
    printf("Child PID = %d\n", getpid());
  }
  else {
    // Parent process
    printf("Parent PID = %d\n", getpid());
    wait(NULL); // Wait for child to finish
  }

  return 0;
}
