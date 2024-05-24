# bpftool
- bpftool is a tool for inspection and simple manipulation of eBPF programs and maps.
- [github.com/libbpf/bpftool](https://github.com/libbpf/bpftool)

## Setup
### Fedora
```bash
sudo dnf install bpftool -y
```

### Debian/Ubuntu (Source)
```bash
# Update packages
sudo apt update

# Install build dependencies
sudo apt install git make pkg-config libelf-dev build-essential zlib1g-dev -y

# Build and install bpftool
git clone https://github.com/libbpf/bpftool.git --recursive
cd bpftool/src/
make
sudo make install
cd ../..
rm -rf bpftool/

# Test bpftool
bpftool --help
```

### Fedora (Source)
```bash
# Install build dependencies
sudo dnf install git make pkg-config elfutils-libelf-devel gcc zlib-devel -y

# Build and install bpftool
git clone https://github.com/libbpf/bpftool.git --recursive
cd bpftool/src/
make
sudo make install
cd ../..
rm -rf bpftool/

# Test bpftool
bpftool --help
```

## Usage
### Generate vmlinux.h Header File
```bash
bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
```
