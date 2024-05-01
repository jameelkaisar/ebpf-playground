#include <stdio.h>
#include <xdp/libxdp.h>
#include <unistd.h> // sleep
#include <stdlib.h> // exit
#include <signal.h> // SIG*
#include <net/if.h> // if_nametoindex

#define BUFFER_SIZE 32

union event {
    __u8 ip_arr[4];
    __u32 ip;
};

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
    printf("\n");
    exit(0);
}

void format_bytes(union event* ev, char* buffer) {
    snprintf(buffer, BUFFER_SIZE, "%u.%u.%u.%u", ev->ip_arr[0], ev->ip_arr[1], ev->ip_arr[2], ev->ip_arr[3]);
}

static int handle_event(void* ctx, void* data, size_t size) {
    union event *ev = data;
    char buff[BUFFER_SIZE];
    format_bytes(ev, buff);
    // Skip private IP addresses
    if (ev->ip_arr[0] == 10) return 0;
    if (ev->ip_arr[0] == 172 && (ev->ip_arr[1] >= 16 && ev->ip_arr[1] <= 31)) return 0;
    if (ev->ip_arr[0] == 192 && ev->ip_arr[1] == 168) return 0;
    printf("IP: %s\n", buff);
    return 0;
}

int main() {
    const char* filename = "xdp_xchng.o";
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

    int rb_fd;
    struct bpf_object *bpf_obj;
    bpf_obj = xdp_program__bpf_obj(prog);
    rb_fd = bpf_object__find_map_fd_by_name(bpf_obj, "ringbuf");
    if (rb_fd < 0) {
        printf("Error, get ring buffer fd from bpf obj failed\n");
        return rb_fd;
    }

    struct ring_buffer *rb = ring_buffer__new(rb_fd, handle_event, NULL, NULL);
    if (!rb) {
        printf("Failed to create ring buffer\n");
        return 1;
    }

    // Detach XDP program when it is interrupted or killed
    signal(SIGINT, int_exit);
    signal(SIGTERM, int_exit);

    // Sleep indefinitely
    while (1) {
        int n = ring_buffer__consume(rb);
        printf("Events Consumed: %d\n", n);
        sleep(1);
    }

    // ring_buffer__free(rb);

    return 0;
}
