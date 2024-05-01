#include <stdio.h>
#include <xdp/libxdp.h>
#include <unistd.h> // sleep
#include <stdlib.h> // exit
#include <signal.h> // SIG*
#include <net/if.h> // if_nametoindex
#include <assert.h> // assert

#define BUFFER_SIZE 32

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

void format_bytes(long bytes, char* buffer) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    const int len = sizeof(units) / sizeof(units[0]);
    double magnitude = bytes;
    int i = 0;
    while (magnitude >= 1024 && i+1 < len) {
        magnitude /= 1024;
        i++;
    }
    snprintf(buffer, BUFFER_SIZE, "%.2f %s", magnitude, units[i]);
}

static void poll_stats(int map_fd, int interval) {
    int ncpus = libbpf_num_possible_cpus();
    if (ncpus < 0) {
        printf("Failed to get possible cpus\n");
        return;
    }

    long values[ncpus], total_num = 0, total_vol = 0;
    int key_num = 0, key_vol = 1;
    char buff_usage[BUFFER_SIZE], buff_speed[BUFFER_SIZE];

    while (1) {
        long num = 0, vol = 0;

        sleep(interval);
        assert(bpf_map_lookup_elem(map_fd, &key_num, values) == 0);
        for (int i = 0; i < ncpus; i++)
            num += values[i];
        num -= total_num;
        total_num += num;
        assert(bpf_map_lookup_elem(map_fd, &key_vol, values) == 0);
        for (int i = 0; i < ncpus; i++)
            vol += values[i];
        vol -= total_vol;
        total_vol += vol;
        format_bytes(total_vol, buff_usage);
        format_bytes(vol / interval, buff_speed);
        printf("\r\033[KUsage: %s (%ld P), Speed: %s/s (%ld P/s)", buff_usage, total_num, buff_speed, num / interval);
        fflush(stdout);
    }
}

int main() {
    const char* filename = "xdp_speed.o";
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

    int map_fd;
    struct bpf_object *bpf_obj;
    bpf_obj = xdp_program__bpf_obj(prog);
    map_fd = bpf_object__find_map_fd_by_name(bpf_obj, "rxcnt");
    if (map_fd < 0) {
        printf("Error, get map fd from bpf obj failed\n");
        return map_fd;
    }

    // Detach XDP program when it is interrupted or killed
    signal(SIGINT, int_exit);
    signal(SIGTERM, int_exit);

    poll_stats(map_fd, 1);

    // Sleep indefinitely
    while (1) sleep(1);

    return 0;
}
