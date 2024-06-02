# eunomia-bpf
- eunomia-bpf is a dynamic loading library/runtime and a compile toolchain framework, aim at helping you build and distribute eBPF programs easier.
- [github.com/eunomia-bpf/eunomia-bpf](https://github.com/eunomia-bpf/eunomia-bpf)

## Setup
### Debian/Ubuntu
```bash
# Install Dependencies
sudo apt install clang llvm wget -y

# Install ecc
wget https://github.com/eunomia-bpf/eunomia-bpf/releases/latest/download/ecc
sudo install -m 755 ecc /usr/bin
rm -rf ecc

# Install ecli
wget https://aka.pw/bpf-ecli -O ecli
sudo install -m 755 ecli /usr/sbin
rm -rf ecli
```

### Fedora
```bash
# Install Dependencies
sudo dnf install clang llvm wget -y

# Install ecc
wget https://github.com/eunomia-bpf/eunomia-bpf/releases/latest/download/ecc
sudo install -m 755 ecc /usr/bin
rm -rf ecc

# Install ecli
wget https://aka.pw/bpf-ecli -O ecli
sudo install -m 755 ecli /usr/sbin
rm -rf ecli
```

## Sample
### Program
```c
// trace_open.bpf.c

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

SEC("tracepoint/syscalls/sys_enter_openat")
int trace_sys_openat(struct pt_regs *ctx) {
    bpf_printk("New File Opened!");
    return 0;
}

char _license[] SEC("license") = "GPL";
```

### Compilation
```bash
ecc trace_open.bpf.c
```

### Load Program
```bash
sudo ecli run package.json
```

### Check Logs
```bash
sudo cat /sys/kernel/debug/tracing/trace
```
