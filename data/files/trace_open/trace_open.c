#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

SEC("tracepoint/syscalls/sys_enter_openat")
int trace_sys_openat(struct pt_regs *ctx) {
    bpf_printk("New File Opened!");
    return 0;
}

char _license[] SEC("license") = "GPL";
