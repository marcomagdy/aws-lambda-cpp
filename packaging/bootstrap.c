// compile with gcc -nostdlib -O2 -o bootstrap bootstrap.c
//

#define NULL ((void *)0)
void _start()
{

    // set environment variable LD_LIBRARY_PATH to $LAMBDA_TASK_ROOT/lib
    static const char * const envp[] = { "LD_LIBRARY_PATH=/var/task/lib", NULL };
    static const char *args[] = { "hello", NULL };
    static const char *filename = "/tmp/task/hello";
    // arm64 inline assembly to call execve syscall (syscall number 221)
    // passing filename, args, envp
    __asm__ __volatile__(
        "mov x8, #0xdd\n\t"
        "mov x0, %0\n\t"
        "mov x1, %1\n\t"
        "mov x2, %2\n\t"
        "mov x16, #0\n\t"
        "svc #0x80\n\t"
        :: "r"(filename), "r"(args), "r"(envp)
    );

    __builtin_unreachable();
}
