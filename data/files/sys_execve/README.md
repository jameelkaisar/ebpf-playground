# execve Tracer (vmlinux.h)
- Traces `execve` system call invocations
- Fetches the process path and the process pwd
- `vmlinux.h` contains all the type definitions that the running Linux kernel uses in its own source code

## Generate vmlinux.h header file
```bash
sudo bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
```

## Compile bpf file
```bash
clang -O2 -g -Wall -target bpf -c sys_execve.c -o sys_execve.o
```

## Compile loader file
```bash
clang -O2 -g -Wall loader.c -o loader.out -lbpf
```

## Execute loader file
```bash
sudo ./loader.out
```

## Test the working
```bash
ls -a

clang -O2 -g -Wall execve_example.c -o execve_example.out
./execve_example.out
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
