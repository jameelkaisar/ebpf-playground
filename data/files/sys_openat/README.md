# openat Tracer (tracepoint)
Modifies `openat` system call invocations
Replaces the `sample.txt` file with `sample_hijacked.txt` file for non-root users
Blocks the access of `sensitive.txt` file for non-root users
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
sudo cat sample.txt
cat sensitive.txt
sudo cat sensitive.txt
```

## Execute loader file
```bash
sudo ./loader.out
```

## Test the working
```bash
cat sample.txt
sudo cat sample.txt
cat sensitive.txt
sudo cat sensitive.txt

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
