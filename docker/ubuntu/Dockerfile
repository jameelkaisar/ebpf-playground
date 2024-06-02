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

# Setup bpftrace
RUN apt install bpftrace -y

# Setup eunomia-bpf
RUN apt install clang llvm wget -y && \
    wget https://github.com/eunomia-bpf/eunomia-bpf/releases/latest/download/ecc && \
    install -m 755 ecc /usr/bin && \
    rm -rf ecc && \
    wget https://github.com/eunomia-bpf/eunomia-bpf/releases/latest/download/ecli && \
    install -m 755 ecli /usr/sbin && \
    rm -rf ecli

# Mount debugfs
CMD mount -t debugfs none /sys/kernel/debug && \
    sleep infinity
