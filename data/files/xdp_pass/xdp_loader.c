#include <stdio.h>
#include <xdp/libxdp.h>
#include <unistd.h> // sleep
#include <stdlib.h> // exit
#include <signal.h> // SIG*
#include <net/if.h> // if_nametoindex

// Global variables
static int ifindex;
struct xdp_program *prog = NULL;
int ret = 1;

// Detach XDP program on exit
static void int_exit(int sig) {
    if (!ret) {
        xdp_program__detach(prog, ifindex, XDP_MODE_SKB, 0);
    }
    xdp_program__close(prog);
    exit(0);
}

int main() {
    const char* filename = "xdp_pass.o";
    const char* secname = "xdp"; // section name

    // Interface name
    // Command: ip a
    const char* ifname = "eth0";

    // Interface name to index
    ifindex = if_nametoindex(ifname);
    if (!ifindex) {
        printf("Failed to get interface index from interface name\n");
        return 1;
    }

    // Load XDP program
    prog = xdp_program__open_file(filename, secname, NULL);
    if (!prog) {
        printf("Failed to load XDP program\n");
        return 1;
    }

    // Attach XDP program to interface with SKB mode
    // Please set ulimit if you got an -EPERM error
    ret = xdp_program__attach(prog, ifindex, XDP_MODE_SKB, 0);
    if (ret) {
        printf("Failed to attach XDP program to %d interface\n", ifindex);
        return ret;
    }

    // Detach XDP program when it is interrupted or killed
    signal(SIGINT, int_exit);
    signal(SIGTERM, int_exit);

    // Sleep indefinitely
    while (1) sleep(1);

    return 0;
}
