// compile with gcc -nostdlib -O2 -o bootstrap bootstrap.c
//
void myexecve(const char* filename, char* const argv[], char* const envp[])
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

int my_main(int argc, char* argv[], char* envp[])
{
        myexecve("/var/task/bin/hello", argv, envp);
        return 0;
}

/* startup code */
asm(".section .text\n"
    ".global _start\n"
    "_start:\n"
    "ldr x0, [sp]\n"              // argc (x0) was in the stack
    "add x1, sp, 8\n"             // argv (x1) = sp
    "lsl x2, x0, 3\n"             // envp (x2) = 8*argc ...
    "add x2, x2, 8\n"             //           + 8 (skip null)
    "add x2, x2, x1\n"            //           + argv
    "and sp, x1, -16\n"           // sp must be 16-byte aligned in the callee
    "bl my_main\n"                   // main() returns the status code, we exit with it
    "");

