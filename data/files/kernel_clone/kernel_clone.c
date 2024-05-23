#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

#include <sys/types.h> // pid_t
#include <asm/ptrace.h> // pt_regs

#define __TARGET_ARCH_x86 // PT_REGS_* is Arch specific
#include <bpf/bpf_tracing.h> // PT_REGS_*


// User Function Definition: fork
// https://elixir.bootlin.com/linux/v6.1/source/tools/include/nolibc/sys.h#L353

// Syscall Definition: sys_fork
// https://elixir.bootlin.com/linux/v6.1/source/kernel/fork.c#L2751

// Kernel Function Definition: kernel_clone
// https://elixir.bootlin.com/linux/v6.1/source/kernel/fork.c#L2630

// Struct Definition: kernel_clone_args
// https://elixir.bootlin.com/linux/v6.1/source/include/linux/sched/task.h#L21

// Return Type: pid_t
// https://elixir.bootlin.com/linux/v6.1/source/include/uapi/asm-generic/posix_types.h#L28
// https://elixir.bootlin.com/linux/v6.1/source/include/linux/types.h#L22

// Struct Definition: pt_regs (x86_64 Register State)
// https://elixir.bootlin.com/linux/latest/source/arch/x86/include/asm/ptrace.h#L12


// ".../linux/sched/task.h"
// $ find / -name task.h 2>/dev/null
struct kernel_clone_args {
	__u64 flags;
	int *pidfd;
	int *child_tid;
	int *parent_tid;
	int exit_signal;
	unsigned long stack;
	unsigned long stack_size;
	unsigned long tls;
	pid_t *set_tid;
	/* Number of elements in *set_tid */
	size_t set_tid_size;
	int cgroup;
	int io_thread;
	int kthread;
	int idle;
	int (*fn)(void *);
	void *fn_arg;
	struct cgroup *cgrp;
	struct css_set *cset;
};


// syscalls(2) — Linux manual page
// https://man7.org/linux/man-pages/man2/syscalls.2.html
// man syscalls

// syscall(2) — Linux manual page
// https://man7.org/linux/man-pages/man2/syscall.2.html
// man syscall

// bpf(2) — Linux manual page
// https://man7.org/linux/man-pages/man2/bpf.2.html
// man bpf

// bpf-helpers(7) — Linux manual page
// https://man7.org/linux/man-pages/man7/bpf-helpers.7.html
// man bpf-helpers


// kernel_clone Kernel Function
// Check if the kernel_clone Function is Available
// sudo cat /proc/kallsyms | grep kernel_clone
// Check if the kernel_clone Function is Blacklisted
// sudo cat /sys/kernel/debug/kprobes/blacklist | grep kernel_clone


SEC("kprobe/kernel_clone")
int kprobe__kernel_clone(struct pt_regs *ctx) {
    char comm[128];
    bpf_get_current_comm(&comm, sizeof(comm));
    pid_t pid = bpf_get_current_pid_tgid();

    // #define PT_REGS_PARM1(x) ((x)->di)
    // #define PT_REGS_PARM2(x) ((x)->si)
    // rdi register stores the first argument of the function in x86_64
    // rsi register stores the second argument of the function in x86_64
    // Check: syscall manpage for more information
    struct kernel_clone_args args;
    int ret = bpf_probe_read_kernel(&args, sizeof(args), (void *)PT_REGS_PARM1(ctx));
    if (ret != 0) return 0;

    // exit_signal is set to SIGCHLD which has a value of 17
    // https://elixir.bootlin.com/linux/v6.1/source/kernel/fork.c#L2755
    bpf_printk("Beg: kernel_clone, Comm: %s, PID: %d, exit_signal: %d", comm, pid, args.exit_signal);

    return 0;
}

SEC("kretprobe/kernel_clone")
int kretprobe__kernel_clone(struct pt_regs *ctx) {
    char comm[128];
    bpf_get_current_comm(&comm, sizeof(comm));
    pid_t pid = bpf_get_current_pid_tgid();

    // Return value is stored in rax register
    // #define PT_REGS_RC(x) ((x)->ax)
    // rax register stores the return value of the function in x86_64
    // Check: man syscall for more information
    pid_t ret = PT_REGS_RC(ctx);

    bpf_printk("End: kernel_clone, Comm: %s, Parent PID: %d, Child PID: %d", comm, pid, ret);

    return 0;
}

char _license[] SEC("license") = "GPL";
