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

# Mount debugfs
CMD mount -t debugfs none /sys/kernel/debug && \
    sleep infinity
