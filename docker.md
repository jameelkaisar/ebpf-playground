# Docker
- Docker is a set of platform as a service products that use OS-level virtualization to deliver software in packages called containers.
- [www.docker.com](https://www.docker.com)

## Setup
- [docs.docker.com/engine/install](https://docs.docker.com/engine/install)

## Build Images
```bash
# Debian
cd docker/debian/
docker build -t ebpf-debian:12 .

# Ubuntu
cd docker/ubuntu/
docker build -t ebpf-ubuntu:22.04 .

# Fedora
cd docker/fedora/
docker build -t ebpf-fedora:40 .
```

## Start Container
```bash
# Debian
docker run -d --privileged --name ebpf-debian ebpf-debian:12
docker exec -it ebpf-debian bash

# Ubuntu
docker run -d --privileged --name ebpf-ubuntu ebpf-ubuntu:22.04
docker exec -it ebpf-ubuntu bash

# Fedora
docker run -d --privileged --name ebpf-fedora ebpf-fedora:40
docker exec -it ebpf-fedora bash
```

## Clean Up
```bash
# Debian
docker stop ebpf-debian
docker rm ebpf-debian
docker rmi ebpf-debian:12

# Ubuntu
docker stop ebpf-ubuntu
docker rm ebpf-ubuntu
docker rmi ebpf-ubuntu:22.04

# Fedora
docker stop ebpf-fedora
docker rm ebpf-fedora
docker rmi ebpf-fedora:40
```
