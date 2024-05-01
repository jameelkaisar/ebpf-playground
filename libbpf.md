# libbpf
- libbpf is a C-based library containing a BPF loader that takes compiled BPF object files and prepares and loads them into the Linux kernel.
- [github.com/libbpf/libbpf](https://github.com/libbpf/libbpf)

## Setup
### Debian/Ubuntu
```bash
sudo apt update
sudo apt install clang gcc-multilib libbpf-dev -y
```

### Fedora
```bash
sudo dnf install clang libbpf libbpf-devel -y
```

### Debian/Ubuntu (Source)
```bash
# Update packages
sudo apt update

# Install build dependencies
sudo apt install git make libelf-dev build-essential pkg-config -y

# Build and install libbpf
git clone https://github.com/libbpf/libbpf.git --recursive
cd libbpf/src/
make
sudo make install
cd ../..
rm -rf libbpf/

# Install runtime dependencies
sudo apt install clang gcc-multilib -y
```

### Fedora (Source)
```bash
# Install build dependencies
sudo dnf install git make zlib-devel elfutils-libelf-devel gcc pkg-config -y

# Build and install libbpf
git clone https://github.com/libbpf/libbpf.git --recursive
cd libbpf/src/
make
sudo make install
cd ../..
rm -rf libbpf/

# Install runtime dependencies
sudo dnf install clang -y
```

## Sample
### Program
```c
// trace_open.c

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
clang -O2 -g -Wall -target bpf -c trace_open.c -o trace_open.o
```
