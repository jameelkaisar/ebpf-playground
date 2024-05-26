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
sudo apt install git make pkg-config libelf-dev build-essential zlib1g-dev llvm -y
sudo apt install man-db docutils-common -y

# Build and install bpftool and docs
git clone https://github.com/libbpf/bpftool.git --recursive
cd bpftool/src/
make
sudo make install
cd ../docs/
make
sudo make install
cd ../..
rm -rf bpftool/

# Test bpftool
sudo bpftool --help
man bpftool
```

### Fedora (Source)
```bash
# Install build dependencies
sudo dnf install git make pkg-config elfutils-libelf-devel gcc zlib-devel llvm -y
sudo dnf install man-db python3-docutils -y

# Build and install bpftool and docs
git clone https://github.com/libbpf/bpftool.git --recursive
cd bpftool/src/
make
sudo make install
cd ../docs/
make
sudo make install
cd ../..
rm -rf bpftool/

# Test bpftool
sudo bpftool --help
man bpftool
```

## Usage
### Generate vmlinux.h Header File
```bash
sudo bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
```
