#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_BUF_SIZE 1024

int main() {
    char buffer[MAX_BUF_SIZE];
    ssize_t bytes_read;

    // Open the trace file
    int fd = open("/sys/kernel/debug/tracing/trace", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open trace file");
        return 1;
    }

    // Read and print new lines indefinitely
    while (1) {
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read == -1) {
            perror("Error reading from trace file");
            close(fd);
            return 1;
        }

        buffer[bytes_read] = '\0';

        if (bytes_read > 0) {
            printf("%s", buffer);
        }

        usleep(100000); // Sleep for 0.1 seconds
    }

    close(fd);
    return 0;
}
