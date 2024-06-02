# openat Tracer
- Traces `openat` system call invocations

## Compile bpf file
```bash
clang -O2 -g -Wall -target bpf -c trace_open.c -o trace_open.o
```

## Compile loader file
```bash
clang -O2 -g -Wall loader.c -o loader.out -lbpf
```

## Execute loader file
```bash
sudo ./loader.out
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
