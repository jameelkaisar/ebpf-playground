FROM fedora:40

# Setup libbpf
RUN dnf install clang libbpf libbpf-devel -y

# Setup libxdp
RUN dnf install libxdp libxdp-devel -y

# Setup bpftool
RUN dnf install bpftool -y

# Install mount
RUN dnf install util-linux-ng -y

# Mount debugfs
CMD mount -t debugfs none /sys/kernel/debug && \
    sleep infinity
