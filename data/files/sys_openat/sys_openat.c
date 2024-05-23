#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

#include <stdint.h> // uint32_t, uint64_t, int32_t, int64_t


// Syscall Definition: sys_openat
// https://elixir.bootlin.com/linux/v6.1/source/fs/open.c#L1337

// Return Type: long
// https://elixir.bootlin.com/linux/v6.1/source/fs/open.c#L1342

// Struct Definition: sys_enter_openat struct format
// sudo cat /sys/kernel/debug/tracing/events/syscalls/sys_enter_openat/format

// Struct Definition: sys_exit_openat struct format
// sudo cat /sys/kernel/debug/tracing/events/syscalls/sys_exit_openat/format


// sys_enter_openat struct
struct sys_enter_openat_args {
    uint64_t unused; // Offset: 8
    int32_t __syscall_nr;
    uint64_t dfd;
    const char * filename;
    uint64_t flags;
    uint64_t mode;
};

// sys_exit_openat struct
struct sys_exit_openat_args {
    uint64_t unused; // Offset: 8
    int32_t __syscall_nr;
    int64_t ret;
};


// syscalls(2) — Linux manual page
// https://man7.org/linux/man-pages/man2/syscalls.2.html
// man syscalls

// bpf(2) — Linux manual page
// https://man7.org/linux/man-pages/man2/bpf.2.html
// man bpf

// bpf-helpers(7) — Linux manual page
// https://man7.org/linux/man-pages/man7/bpf-helpers.7.html
// man bpf-helpers


// openat Syscall
// Check if the openat Tracepoint is Available
// sudo cat /sys/kernel/debug/tracing/available_events | grep openat
// Check if the openat Tracepoint is Enabled
// sudo cat /sys/kernel/debug/tracing/events/syscalls/sys_enter_openat/enable


SEC("tracepoint/syscalls/sys_enter_openat")
int trace_sys_enter_openat(struct sys_enter_openat_args *ctx) {
    // Read the filename
    char filename[256];
    bpf_probe_read_user_str(filename, sizeof(filename), ctx->filename);

    // Check if the filename is "sample.txt"
    if (bpf_strncmp(filename, 10, "sample.txt") == 0) {
        bpf_printk("Hijacking openat syscall");
        // Change the filename to "sample_hijacked.txt"
        bpf_probe_write_user((char*)ctx->filename, "sample_hijacked.txt", 20);
    }

    bpf_printk("Enter: syscall=%d, filename=%s", ctx->__syscall_nr, ctx->filename);
    return 0;
}

SEC("tracepoint/syscalls/sys_exit_openat")
int trace_sys_exit_openat(struct sys_exit_openat_args *ctx) {
    bpf_printk("Exit: syscall=%d, ret=%ld", ctx->__syscall_nr, ctx->ret);
    return 0;
}

char _license[] SEC("license") = "GPL";
