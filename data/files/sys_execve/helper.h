// Source: StackOverflow
// Question: how to get the full path of the process executable file based on ebpf?
// Question Link: https://stackoverflow.com/q/75877938 (by szchenrong)
// Solution Link: https://stackoverflow.com/a/77308209 (by sc07kvm)



// Generate vmlinux.h using bpftool
// bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
#include "vmlinux.h"

#include <bpf/bpf_helpers.h>
#include <bpf/bpf_core_read.h>



#define statfunc static __attribute__((__always_inline__))



#define MAX_PERCPU_ARRAY_SIZE (1 << 15)
#define HALF_PERCPU_ARRAY_SIZE (MAX_PERCPU_ARRAY_SIZE >> 1)
#define LIMIT_PERCPU_ARRAY_SIZE(x) ((x) & (MAX_PERCPU_ARRAY_SIZE - 1))
#define LIMIT_HALF_PERCPU_ARRAY_SIZE(x) ((x) & (HALF_PERCPU_ARRAY_SIZE - 1))

struct buffer {
  u8 data[MAX_PERCPU_ARRAY_SIZE];
};

struct {
  __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
  __type(key, u32);
  __type(value, struct buffer);
  __uint(max_entries, 1);
} heaps_map SEC(".maps");

statfunc struct buffer *get_buffer() {
  u32 zero = 0;
  return (struct buffer *)bpf_map_lookup_elem(&heaps_map, &zero);
}



#define EFAULT 14 /* Bad address */

#define MAX_PATH_SIZE 4096 // PATH_MAX from <linux/limits.h>
#define LIMIT_PATH_SIZE(x) ((x) & (MAX_PATH_SIZE - 1))
#define MAX_PATH_COMPONENTS 20

statfunc long get_path_str_from_path(u_char **path_str, struct path *path, struct buffer *out_buf) {

  long ret;
  struct dentry *dentry, *dentry_parent, *dentry_mnt;
  struct vfsmount *vfsmnt;
  struct mount *mnt, *mnt_parent;
  const u_char *name;
  size_t name_len;

  dentry = BPF_CORE_READ(path, dentry);
  vfsmnt = BPF_CORE_READ(path, mnt);
  mnt = container_of(vfsmnt, struct mount, mnt);
  mnt_parent = BPF_CORE_READ(mnt, mnt_parent);

  size_t buf_off = HALF_PERCPU_ARRAY_SIZE;

#pragma unroll
  for (int i = 0; i < MAX_PATH_COMPONENTS; i++) {

    dentry_mnt = BPF_CORE_READ(vfsmnt, mnt_root);
    dentry_parent = BPF_CORE_READ(dentry, d_parent);

    if (dentry == dentry_mnt || dentry == dentry_parent) {
      if (dentry != dentry_mnt) {
        // We reached root, but not mount root - escaped?
        break;
      }
      if (mnt != mnt_parent) {
        // We reached root, but not global root - continue with mount point path
        dentry = BPF_CORE_READ(mnt, mnt_mountpoint);
        mnt_parent = BPF_CORE_READ(mnt, mnt_parent);
        vfsmnt = __builtin_preserve_access_index(&mnt->mnt);
        continue;
      }
      // Global root - path fully parsed
      break;
    }

    // Add this dentry name to path
    name_len = LIMIT_PATH_SIZE(BPF_CORE_READ(dentry, d_name.len));
    name = BPF_CORE_READ(dentry, d_name.name);

    name_len = name_len + 1; // add slash
    // Is string buffer big enough for dentry name?
    if (name_len > buf_off) { break; }
    volatile size_t new_buff_offset = buf_off - name_len; // satisfy verifier
    ret = bpf_probe_read_kernel_str(
      &(out_buf->data[LIMIT_HALF_PERCPU_ARRAY_SIZE(new_buff_offset) // satisfy verifier
    ]),
      name_len,
      name);
    if (ret < 0) { return ret; }

    if (ret > 1) {
      buf_off -= 1;                                    // remove null byte termination with slash sign
      buf_off = LIMIT_HALF_PERCPU_ARRAY_SIZE(buf_off); // satisfy verifier
      out_buf->data[buf_off] = '/';
      buf_off -= ret - 1;
      buf_off = LIMIT_HALF_PERCPU_ARRAY_SIZE(buf_off); // satisfy verifier
    } else {
      // If sz is 0 or 1 we have an error (path can't be null nor an empty string)
      break;
    }
    dentry = dentry_parent;
  }

  // Is string buffer big enough for slash?
  if (buf_off != 0) {
    // Add leading slash
    buf_off -= 1;
    buf_off = LIMIT_HALF_PERCPU_ARRAY_SIZE(buf_off); // satisfy verifier
    out_buf->data[buf_off] = '/';
  }

  // Null terminate the path string
  out_buf->data[HALF_PERCPU_ARRAY_SIZE - 1] = 0;
  *path_str = &out_buf->data[buf_off];
  return HALF_PERCPU_ARRAY_SIZE - buf_off - 1;
}



// My wrapper function mimicking bpf_d_path
statfunc void my_bpf_d_path(u_char **file_path, struct path *path) {
  struct buffer *string_buf = get_buffer();
  if (string_buf == NULL) return;
  get_path_str_from_path(file_path, path, string_buf);
}
