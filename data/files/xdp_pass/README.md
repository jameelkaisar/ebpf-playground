# XDP Pass
Log Size of All Ingress Network Packets

## Compile bpf file
```bash
clang -O2 -g -Wall -target bpf -c xdp_pass.c -o xdp_pass.o
```

## Compile loader file
```bash
clang -O2 -g -Wall xdp_loader.c -o xdp_loader.out -lbpf -lxdp
```

## Execute loader file
```bash
sudo ./xdp_loader.out
```

## Download a test file
```bash
wget -O /dev/null https://link.testfile.org/15MB
```

## Read logs
### Read logs (directly)
```bash
sudo cat /sys/kernel/debug/tracing/trace
sudo cat /sys/kernel/tracing/trace
```

### Read logs (indefinitely)
```bash
# Compile log reader
clang -O2 -g -Wall ../common/log_reader.c -o ../common/log_reader.out

# Execute log reader
sudo ./../common/log_reader.out
```
