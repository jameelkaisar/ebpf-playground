# bpftrace
- bpftrace is a high-level tracing language for Linux eBPF.
- [github.com/bpftrace/bpftrace](https://github.com/bpftrace/bpftrace)

## Setup
### Debian/Ubuntu
```bash
sudo apt install bpftrace -y
```

### Fedora
```bash
sudo dnf install bpftrace -y
```

## Usage
### Trace Bash Commands
```bash
sudo bpftrace -e 'uretprobe:/bin/bash:readline { printf("%-6d %s\n", pid, str(retval)); }'
```
