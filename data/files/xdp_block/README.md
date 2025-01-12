# XDP Block
- Block a domain with static DNS A records

## Compile bpf file
```bash
clang -O2 -g -Wall -target bpf -c xdp_block.c -o xdp_block.o
```

## Compile loader file
```bash
clang -O2 -g -Wall xdp_loader.c -o xdp_loader.out -lbpf -lxdp
```

## Execute loader file
```bash
sudo ./xdp_loader.out <domain-name>
sudo ./xdp_loader.out jameelkaisar.com
```

## Open a website
```bash
curl https://<domain-name>
curl https://jameelkaisar.com
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
