FROM fedora:40

# Setup libbpf
RUN dnf install clang libbpf libbpf-devel -y

# Setup libxdp
RUN dnf install libxdp libxdp-devel -y

# Setup bpftool
RUN dnf install bpftool -y

# Install mount
RUN dnf install util-linux-ng -y

# Setup bpftrace
RUN dnf install bpftrace -y

# Setup eunomia-bpf
RUN dnf install clang llvm wget -y && \
    wget https://github.com/eunomia-bpf/eunomia-bpf/releases/latest/download/ecc && \
    install -m 755 ecc /usr/bin && \
    rm -rf ecc && \
    wget https://github.com/eunomia-bpf/eunomia-bpf/releases/latest/download/ecli && \
    install -m 755 ecli /usr/sbin && \
    rm -rf ecli

# Mount debugfs
CMD mount -t debugfs none /sys/kernel/debug && \
    sleep infinity
