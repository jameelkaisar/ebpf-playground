#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>
#include <linux/ip.h> // struct iphdr
#include <bpf/bpf_endian.h> // bpf_ntohs

struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 256 * 1024 /* 256 KB */);
} ringbuf SEC(".maps");

union event {
    __u8 ip_arr[4];
    __u32 ip;
};

SEC("xdp")
int xdp_xchng_prog(struct xdp_md* ctx) {
    void* data = (void*)(long)ctx->data;
    void* data_end = (void*)(long)ctx->data_end;

    // Check if the packet is large enough
    if (data + sizeof(struct ethhdr) > data_end)
        return XDP_DROP;
    
    struct ethhdr *eth = data;
    
    // We only need IP Packets
    // eth->h_proto is 2 bytes
    // bpf_ntohs takes care of byte order
    if (bpf_ntohs(eth->h_proto) != ETH_P_IP)
        return XDP_PASS;

    // Check if the packet is large enough
    if (data + sizeof(struct ethhdr) + sizeof(struct iphdr) > data_end)
        return XDP_DROP;

    struct iphdr *iph = data + sizeof(struct ethhdr);

    // We only need TCP Packets
    if (iph->protocol != IPPROTO_TCP)
        return XDP_PASS;
    
    // Reserve space in the ring buffer    
    union event *ev = bpf_ringbuf_reserve(&ringbuf, sizeof(union event), 0);
    if (!ev)
        return XDP_PASS;

    // Copy the IP address
    ev->ip = iph->saddr;

    // Submit the event
    bpf_ringbuf_submit(ev, 0);

    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
