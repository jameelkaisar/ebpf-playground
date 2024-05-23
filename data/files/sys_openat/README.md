# openat Tracer (tracepoint)
Traces `openat` system call invocations
Modifies the `openat` system call argument with `sample_hijacked.txt` if it is `sample.txt`
`tracepoints` are stable over the versions of the Linux kernel

## Compile bpf file
```bash
clang -O2 -g -Wall -target bpf -c sys_openat.c -o sys_openat.o
```

## Compile loader file
```bash
clang -O2 -g -Wall loader.c -o loader.out -lbpf
```

## Test the working
```bash
cat sample.txt
cat sample_hijacked.txt
```

## Execute loader file
```bash
sudo ./loader.out
```

## Test the working
```bash
cat sample.txt
cat sample_hijacked.txt

# This will not be hijacked
cat ../sys_openat/sample.txt
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
