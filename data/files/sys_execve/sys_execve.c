// Generate vmlinux.h using bpftool
// bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
#include "vmlinux.h"
#include "helper.h" // my_bpf_d_path

#include <bpf/bpf_helpers.h>
#include <bpf/bpf_core_read.h> // BPF_CORE_READ


// Syscall Definition: sys_execve
// https://elixir.bootlin.com/linux/v6.1/source/fs/exec.c#L2087

// Return Type: int
// https://elixir.bootlin.com/linux/v6.1/source/fs/exec.c#L2092

// Struct Definition: sys_enter_execve struct format
// sudo cat /sys/kernel/debug/tracing/events/syscalls/sys_enter_execve/format

// Struct Definition: task_struct
// https://elixir.bootlin.com/linux/v6.1/source/include/linux/sched.h#L737

// Struct Definition: pt_regs (x86_64 Register State)
// https://elixir.bootlin.com/linux/latest/source/arch/x86/include/asm/ptrace.h#L12

// Struct Definition: fs_struct (task_struct -> fs)
// https://elixir.bootlin.com/linux/v6.1/source/include/linux/fs_struct.h#L9

// Struct Definition: path (fs_struct -> pwd)
// https://elixir.bootlin.com/linux/v6.1/source/include/linux/path.h#L8

// Struct Definition: mm_struct (task_struct -> mm)
// https://elixir.bootlin.com/linux/v6.1/source/include/linux/mm_types.h#L512

// Struct Definition: file (mm_struct -> exe_file)
// https://elixir.bootlin.com/linux/v6.1/source/include/linux/fs.h#L940

// Struct Definition: path (file -> f_path)
// https://elixir.bootlin.com/linux/v6.1/source/include/linux/path.h#L8


// Function Definition: bpf_d_path
// https://elixir.bootlin.com/linux/v6.1/source/kernel/trace/bpf_trace.c#L900
// Error: unknown func bpf_d_path#147
// bpf_d_path function is very restricted, can't be used in most of the BPF programs
// https://elixir.bootlin.com/linux/v6.1/source/kernel/trace/bpf_trace.c#L919


// sys_enter_execve struct
struct sys_enter_execve_args {
    uint64_t unused; // Offset: 8
    int32_t __syscall_nr;
    const char * filename;
    const char *const * argv;
    const char *const * envp;
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


// execve Syscall
// Check if the execve Tracepoint is Available
// sudo cat /sys/kernel/debug/tracing/available_events | grep execve
// Check if the execve Tracepoint is Enabled
// sudo cat /sys/kernel/debug/tracing/events/syscalls/sys_enter_execve/enable


SEC("tracepoint/syscalls/sys_enter_execve")
int trace_sys_enter_execve(struct sys_enter_execve_args *ctx) {
    // Access the task_struct from bpf context
    struct task_struct *task = bpf_get_current_task_btf();

    // Access the pt_regs struct from the task_struct
    // struct pt_regs *regs = (struct pt_regs *)bpf_task_pt_regs(task);

    // Access the pid of the current process
    // uint32_t pid = bpf_get_current_pid_tgid() >> 32;

    // Access the uid of the current process
    // uint32_t uid = bpf_get_current_uid_gid();

    // Access the path of the current process
    struct path proc_path = BPF_CORE_READ(task, mm, exe_file, f_path);
    // Error: unknown func bpf_d_path#147
    // char proc_path_str[256];
    // bpf_d_path(&proc_path, proc_path_str, sizeof(proc_path_str));
    // Solution: Use the helper function my_bpf_d_path
    u_char *proc_path_str = NULL;
    my_bpf_d_path(&proc_path_str, &proc_path);
    if (proc_path_str == NULL) return 0;
    bpf_printk("Process Path: %s", proc_path_str);

    // Access the path of the present working directory
    struct path pwd_path = BPF_CORE_READ(task, fs, pwd);
    // Error: unknown func bpf_d_path#147
    // char pwd_path_str[256];
    // bpf_d_path(&pwd_path, pwd_path_str, sizeof(pwd_path_str));
    // Solution: Use the helper function my_bpf_d_path
    u_char *pwd_path_str = NULL;
    my_bpf_d_path(&pwd_path_str, &pwd_path);
    if (pwd_path_str == NULL) return 0;
    bpf_printk("PWD Path: %s", pwd_path_str);

    // Access the executable filename
    char exec_file[256];
    bpf_probe_read_user_str(exec_file, sizeof(exec_file), ctx->filename);
    bpf_printk("Exec Filename: %s", exec_file);

    return 0;
}

char _license[] SEC("license") = "GPL";
