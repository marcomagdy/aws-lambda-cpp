// compile with gcc -nostdlib -O2 -o bootstrap bootstrap.c
static void myexecve(const char* filename, char* const argv[], char* const envp[])
{
    __asm__ __volatile__(
        "mov x8, #0xdd\n\t"
        "mov x0, %0\n\t"
        "mov x1, %1\n\t"
        "mov x2, %2\n\t"
        "svc #0x80\n\t"
        :: "r"(filename), "r"(argv), "r"(envp)
    );

    __builtin_unreachable();

}

__attribute__((noinline))
int main(int argc, char* argv[], char* envp[])
{
        myexecve("/var/task/bin/hello", argv, envp);
        return 0;
}

extern "C" void premain(long *p)
{
        int argc = p[0];
        char **argv = (char **)(p+1);
        char **envp = argv + argc + 1;
        main(argc, argv, envp);
        __builtin_unreachable();

}

/* startup code */
asm (".section .text\n"
     ".global _start\n"
     "_start:\n"
     "mov x0, sp\n"              // argc (x0) was in the stack
     "and sp, x0, -16\n"         // sp must be 16-byte aligned in the callee
     "bl premain\n"
     );

