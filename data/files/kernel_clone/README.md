# kernel_clone Tracer (kprobe and kretprobe)
Traces `kernel_clone` kernel function invocations

## Compile bpf file
```bash
clang -O2 -g -Wall -target bpf -c kernel_clone.c -o kernel_clone.o
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
cat --version
cat --version

echo $$
bash -c 'echo $$'

clang -O2 -g -Wall fork_example.c -o fork_example.out
./fork_example.out
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
