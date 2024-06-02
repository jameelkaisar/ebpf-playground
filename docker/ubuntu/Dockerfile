FROM ubuntu:22.04

# Update Package Repository
RUN apt update

# Setup libbpf
RUN apt install clang gcc-multilib libbpf-dev -y

# Setup libxdp
RUN apt install git make pkg-config build-essential clang m4 zlib1g-dev libelf-dev libpcap-dev -y && \
    git clone https://github.com/xdp-project/xdp-tools.git && \
    cd xdp-tools/ && \
    make libxdp && \
    make libxdp_install && \
    cd .. && \
    rm -rf xdp-tools/

# Setup bpftool
RUN apt install wget -y && \
    wget https://github.com/libbpf/bpftool/releases/download/v7.4.0/bpftool-v7.4.0-amd64.tar.gz && \
    tar -xzf bpftool-v7.4.0-amd64.tar.gz && \
    install -m 755 bpftool /usr/sbin && \
    rm -rf bpftool-v7.4.0-amd64.tar.gz bpftool

# Mount debugfs
CMD mount -t debugfs none /sys/kernel/debug && \
    sleep infinity
