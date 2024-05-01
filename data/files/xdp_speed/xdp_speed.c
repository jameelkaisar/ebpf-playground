#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>

struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
    __type(key, __u32);
    __type(value, long);
    __uint(max_entries, 2);
} rxcnt SEC(".maps");

SEC("xdp")
int xdp_speed_prog(struct xdp_md* ctx) {
    void* data = (void*)(long)ctx->data;
    void* data_end = (void*)(long)ctx->data_end;
    int pkt_sz = data_end - data;

    __u32 key_num = 0, key_vol = 1;
    long *value_num, *value_vol;

    if (data + sizeof(struct ethhdr) > data_end)
        return XDP_DROP;

    value_num = bpf_map_lookup_elem(&rxcnt, &key_num);
    if (value_num) *value_num += 1;

    value_vol = bpf_map_lookup_elem(&rxcnt, &key_vol);
    if (value_vol) *value_vol += pkt_sz;

    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
