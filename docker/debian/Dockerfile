FROM debian:12

# Update Package Repository
RUN apt update

# Setup libbpf
RUN apt install clang gcc-multilib libbpf-dev -y

# Setup libxdp
RUN apt install libxdp-dev -y

# Setup bpftool
RUN apt install wget -y && \
    wget https://github.com/libbpf/bpftool/releases/download/v7.4.0/bpftool-v7.4.0-amd64.tar.gz && \
    tar -xzf bpftool-v7.4.0-amd64.tar.gz && \
    install -m 755 bpftool /usr/sbin && \
    rm -rf bpftool-v7.4.0-amd64.tar.gz bpftool

# Setup bpftrace
RUN apt install bpftrace -y

# Setup eunomia-bpf
RUN apt install clang llvm wget fuse3 -y && \
    wget https://github.com/eunomia-bpf/eunomia-bpf/releases/latest/download/ecc && \
    install -m 755 ecc /usr/bin && \
    rm -rf ecc && \
    wget https://github.com/eunomia-bpf/eunomia-bpf/releases/latest/download/ecli && \
    install -m 755 ecli /usr/sbin && \
    rm -rf ecli

# Mount debugfs
CMD mount -t debugfs none /sys/kernel/debug && \
    sleep infinity
