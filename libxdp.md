# libxdp
- libxdp provides two primary features on top of libbpf. The first is the ability to load multiple XDP programs in sequence on a single network device. The second main feature is helper functions for configuring AF_XDP sockets as well as reading and writing packets from these sockets.
- [github.com/xdp-project/xdp-tools/tree/master/lib/libxdp](https://github.com/xdp-project/xdp-tools/tree/master/lib/libxdp)
- Prerequisite: libbpf

## Setup
### Debian
```bash
sudo apt update
sudo apt install libxdp-dev -y
```

### Fedora
```bash
sudo dnf install libxdp libxdp-devel -y
```

### Debian/Ubuntu (Source)
```bash
# Update packages
sudo apt update

# Install build dependencies
sudo apt install git make pkg-config build-essential clang m4 zlib1g-dev libelf-dev libpcap-dev -y

# Build and install libxdp
git clone https://github.com/xdp-project/xdp-tools.git
cd xdp-tools/
make libxdp
sudo make libxdp_install
cd ..
rm -rf xdp-tools/
```

### Fedora (Source)
```bash
# Install build dependencies
sudo dnf install git make pkg-config gcc clang llvm m4 zlib-devel elfutils-libelf-devel libpcap-devel -y

# Build and install libxdp
git clone https://github.com/xdp-project/xdp-tools.git
cd xdp-tools/
make libxdp
sudo make libxdp_install
cd ..
rm -rf xdp-tools/
```

## Sample
### Program
```c
// xdp_pass.c

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

SEC("xdp")
int xdp_pass_prog(struct xdp_md* ctx) {
    void* data = (void*)(long)ctx->data;
    void* data_end = (void*)(long)ctx->data_end;
    int pkt_sz = data_end - data;

    bpf_printk("Packet Size: %d", pkt_sz);
    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
```

### Compilation
```bash
clang -O2 -g -Wall -target bpf -c xdp_pass.c -o xdp_pass.o
```
