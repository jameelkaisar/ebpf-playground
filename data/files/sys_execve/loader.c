#include <stdio.h>
#include <bpf/libbpf.h>
#include <unistd.h> // sleep

int main() {
    const char* filename = "sys_execve.o";
    const char* progname = "trace_sys_enter_execve"; // function name

    // Open bpf object
    struct bpf_object* bpfObject = bpf_object__open(filename);
    if (!bpfObject) {
        printf("Failed to open %s\n", filename);
        return 1;
    }

    // Load bpf object
    int err = bpf_object__load(bpfObject);
    if (err) {
        printf("Failed to load %s\n", filename);
        return 1;
    }

    // Find bpf program
    struct bpf_program* bpfProg = bpf_object__find_program_by_name(bpfObject, progname);
    if (!bpfProg) {
        printf("Failed to find %s\n", progname);
        return 1;
    }

    // Attach bpf program
    bpf_program__attach(bpfProg);

    // Sleep indefinitely
    while (1) sleep(1);

    return 0;
}
