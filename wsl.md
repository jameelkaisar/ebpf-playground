# WSL (Windows Subsystem for Linux)
- Windows Subsystem for Linux is a feature of Microsoft Windows that allows developers to run a Linux environment without the need for a separate virtual machine or dual booting.
- [learn.microsoft.com/en-us/windows/wsl/about](https://learn.microsoft.com/en-us/windows/wsl/about)

## Setup
- [learn.microsoft.com/en-us/windows/wsl/install](https://learn.microsoft.com/en-us/windows/wsl/install)

## Rebuild the WSL2 Kernel (with the missing kernel modules)
```bash
# Access WSL
wsl

# Update Package Repository
sudo apt update

# Install Dependencies
sudo apt install build-essential flex bison dwarves libssl-dev libelf-dev bc python3 pahole git make -y

# Checkout Repository
export KERNEL_VERSION=$(uname -r | cut -d '-' -f 1)
git clone https://github.com/microsoft/WSL2-Linux-Kernel.git -b linux-msft-wsl-$KERNEL_VERSION --depth 1
cd WSL2-Linux-Kernel

# Compile and Install
cp Microsoft/config-wsl .config
make -j$(nproc) oldconfig && make -j$(nproc) prepare
make -j$(nproc) scripts
make -j$(nproc) modules
sudo make -j$(nproc) modules_install

# Copy Compiled Files
sudo cp -r /lib/modules/$KERNEL_VERSION-microsoft-standard-WSL2+/* /lib/modules/$KERNEL_VERSION-microsoft-standard-WSL2/

# Exit WSL
exit

# Shutdown WSL
wsl --shutdown

# Access WSL
wsl

# Mount debugfs
sudo mount -t debugfs debugfs /sys/kernel/debug

# Test debugfs
sudo cat /sys/kernel/debug/tracing/trace
```
